// Copyright (C) 2009 Timothy Brownawell <tbrownaw@prjek.net>
//
// This program is made available under the GNU GPL version 2.0 or
// greater. See the accompanying file COPYING for details.
//
// This program is distributed WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.

#include "../base.hh"
#include "../app_state.hh"
#include "../key_store.hh"
#include "../database.hh"
#include "../keys.hh"
#include "../lazy_rng.hh"
#include "../lua_hooks.hh"
#include "automate_session.hh"
#include "netsync_session.hh"
#include "../options.hh"
#include "../project.hh"
#include "../vocab_cast.hh"
#include "session.hh"
#include "automate_session.hh"
#include "netsync_session.hh"

using std::make_shared;
using std::shared_ptr;
using std::string;

using boost::lexical_cast;

static const var_domain known_servers_domain = var_domain("known-servers");

size_t session::session_num = 0;

session::session(app_state & app, project_t & project,
                 key_store & keys,
                 protocol_voice voice,
                 std::string const & peer,
                 shared_ptr<Netxx::StreamBase> && sock) :
  session_base(voice, peer, move(sock)),
  version(app.opts.max_netsync_version),
  max_version(app.opts.max_netsync_version),
  min_version(app.opts.min_netsync_version),
  use_transport_auth(!app.opts.no_transport_auth),
  signing_key(keys.signing_key),
  cmd_in(0),
  armed(false),
  received_remote_key(false),
  session_key(constants::netsync_key_initializer),
  read_hmac(netsync_session_key(constants::netsync_key_initializer),
            use_transport_auth),
  write_hmac(netsync_session_key(constants::netsync_key_initializer),
             use_transport_auth),
  authenticated(false),
  completed_hello(false),
  error_code(0),
  session_id(++session_num),
  app(app),
  project(project),
  keys(keys),
  peer(peer),
  unnoted_bytes_in(0),
  unnoted_bytes_out(0)
{
  if (!app.opts.max_netsync_version_given)
    {
      max_version = constants::netcmd_current_protocol_version;
      version = max_version;
    }
  if (!app.opts.min_netsync_version_given)
    min_version = constants::netcmd_minimum_protocol_version;
}

session::~session()
{
  if (wrapped)
    wrapped->on_end(session_id);
}

void session::set_inner(shared_ptr<wrapped_session> && wrapped)
{
  this->wrapped = wrapped;
}

id
session::mk_nonce()
{
  I(this->saved_nonce().empty());
  char buf[constants::merkle_hash_length_in_bytes];

#if BOTAN_VERSION_CODE >= BOTAN_VERSION_CODE_FOR(1,7,7)
  lazy_rng::get().randomize(reinterpret_cast<Botan::byte *>(buf),
                            constants::merkle_hash_length_in_bytes);
#else
  Botan::Global_RNG::randomize(reinterpret_cast<Botan::byte *>(buf),
                               constants::merkle_hash_length_in_bytes);
#endif
  this->saved_nonce = id(string(buf, buf + constants::merkle_hash_length_in_bytes),
                         origin::internal);
  I(this->saved_nonce().size() == constants::merkle_hash_length_in_bytes);
  return this->saved_nonce;
}

void
session::set_session_key(string const & key)
{
  session_key = netsync_session_key(key, origin::internal);
  read_hmac.set_key(session_key);
  write_hmac.set_key(session_key);
}

void
session::set_session_key(rsa_oaep_sha_data const & hmac_key_encrypted)
{
  MM(use_transport_auth);
  if (use_transport_auth)
    {
      MM(signing_key);
      string hmac_key;
      keys.decrypt_rsa(signing_key, hmac_key_encrypted, hmac_key);
      set_session_key(hmac_key);
    }
}

bool session::arm()
{
  if (!armed)
    {
      // Don't pack the buffer unnecessarily.
      if (output_overfull())
        return false;

      if (cmd_in.read(min_version, max_version, inbuf, read_hmac))
        {
          L(FL("armed with netcmd having code '%d'") % cmd_in.get_cmd_code());
          armed = true;
        }
    }
  return armed;
}

void session::begin_service()
{
  netcmd cmd(0);
  cmd.write_usher_cmd(utf8("", origin::internal));
  write_netcmd(cmd);
}

bool session::do_work(transaction_guard & guard)
{
  try
    {
      arm();
      bool is_goodbye = armed && cmd_in.get_cmd_code() == bye_cmd;
      bool is_error = armed && cmd_in.get_cmd_code() == error_cmd;
      if (completed_hello && !is_goodbye && !is_error)
        {
          if (encountered_error)
            return true;
          else
            {
              if (armed)
                L(FL("doing work for peer '%s' with '%d' netcmd")
                  % get_peer() % cmd_in.get_cmd_code());
              else
                L(FL("doing work for peer '%s' with no netcmd")
                  % get_peer());
              bool ok = wrapped->do_work(guard, armed ? &cmd_in : 0);
              armed = false;
              if (ok)
                {
                  if (voice == client_voice
                      && protocol_state == working_state
                      && wrapped->finished_working())
                    {
                      protocol_state = shutdown_state;
                      guard.do_checkpoint();
                      queue_bye_cmd(0);
                    }
                }
              return ok;
            }
        }
      else
        {
          if (!armed)
            return true;
          armed = false;
          switch (cmd_in.get_cmd_code())
            {
            case usher_cmd:
              {
                utf8 msg;
                cmd_in.read_usher_cmd(msg);
                if (msg().size())
                  {
                    if (msg()[0] == '!')
                      P(F("received warning from usher: %s") % msg().substr(1));
                    else
                      L(FL("received greeting from usher: %s") % msg().substr(1));
                  }
                netcmd cmdout(version);
                cmdout.write_usher_reply_cmd(utf8(peer_id, origin::internal),
                                             wrapped->usher_reply_data());
                write_netcmd(cmdout);
                L(FL("sent reply."));
                return true;
              }
            case usher_reply_cmd:
              {
                u8 client_version;
                utf8 server;
                string pattern;
                cmd_in.read_usher_reply_cmd(client_version, server, pattern);

                // netcmd::read() has already checked that the client isn't too old
                if (client_version < max_version)
                  {
                    version = client_version;
                  }
                L(FL("client has maximum version %d, using %d")
                  % widen<u32>(client_version) % widen<u32>(version));
                netcmd cmd(version);

                key_name name;
                keypair kp;
                if (use_transport_auth)
                  {
                    keys.get_key_pair(signing_key, name, kp);
                    cmd.write_hello_cmd(name, kp.pub, mk_nonce());
                  }
                else
                  {
                    cmd.write_hello_cmd(name, kp.pub, mk_nonce());
                  }
                write_netcmd(cmd);
                return true;
              }
            case hello_cmd:
              { // need to ask wrapped what to reply with (we're a client)
                u8 server_version;
                key_name their_keyname;
                rsa_pub_key their_key;
                id nonce;
                cmd_in.read_hello_cmd(server_version, their_keyname,
                                      their_key, nonce);
                hello_nonce = nonce;

                I(!received_remote_key);
                I(saved_nonce().empty());

                // version sanity has already been checked by netcmd::read()
                L(FL("received hello command; setting version from %d to %d")
                  % widen<u32>(get_version())
                  % widen<u32>(server_version));
                version = server_version;

                if (use_transport_auth)
                  {
                    remote_peer_key_id = key_hash_code(their_keyname,
                                                       their_key);

                    var_value printable_key_hash;
                    {
                      hexenc<id> encoded_key_hash;
                      encode_hexenc(remote_peer_key_id.inner(), encoded_key_hash);
                      printable_key_hash = typecast_vocab<var_value>(encoded_key_hash);
                    }
                    L(FL("server key has name %s, hash %s")
                      % their_keyname % printable_key_hash);
                    var_key their_key_key(known_servers_domain,
                                          var_name(get_peer(), origin::internal));
                    if (project.db.var_exists(their_key_key))
                      {
                        var_value expected_key_hash
                          = project.db.get_var(their_key_key);
                        if (expected_key_hash != printable_key_hash)
                          {
                            P(F("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"
                                "@ WARNING: SERVER IDENTIFICATION HAS CHANGED              @\n"
                                "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"
                                "IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING NASTY\n"
                                "It is also possible that the server key has just been changed.\n"
                                "Remote host sent key %s,\n"
                                "I expected %s\n"
                                "'%s unset %s %s' overrides this check")
                              % printable_key_hash
                              % expected_key_hash
                              % prog_name % their_key_key.first % their_key_key.second);
                            E(false, origin::network, F("server key changed"));
                          }
                      }
                    else
                      {
                        P(F("first time connecting to server %s.\n"
                            "I'll assume it's really them, but you might want to double-check\n"
                            "their key's fingerprint: %s")
                          % get_peer()
                          % printable_key_hash);
                        project.db.set_var(their_key_key, printable_key_hash);
                      }

                    if (!project.db.public_key_exists(remote_peer_key_id))
                      {
                        // this should now always return true since we just checked
                        // for the existence of this particular key
                        I(project.db.put_key(their_keyname, their_key));
                        W(F("saving public key for %s to database") % their_keyname);
                      }
                    {
                      hexenc<id> hnonce;
                      encode_hexenc(nonce, hnonce);
                      L(FL("received 'hello' netcmd from server '%s' with nonce '%s'")
                        % printable_key_hash % hnonce);
                    }

                    I(project.db.public_key_exists(remote_peer_key_id));

                    // save their identity
                    received_remote_key = true;
                  }

                wrapped->request_service();

              }
              return true;

            case anonymous_cmd:
            case auth_cmd:
            case automate_cmd:
              return handle_service_request();

            case confirm_cmd:
              {
                authenticated = true; // maybe?
                completed_hello = true;
                wrapped->accept_service();
              }
              return true;

            case bye_cmd:
              {
                u8 phase;
                cmd_in.read_bye_cmd(phase);
                return process_bye_cmd(phase, guard);
              }
            case error_cmd:
              {
                string errmsg;
                cmd_in.read_error_cmd(errmsg);

                // "xxx string" with xxx being digits means there's an error code
                if (errmsg.size() > 4 && errmsg.substr(3,1) == " ")
                  {
                    try
                      {
                        int err = boost::lexical_cast<int>(errmsg.substr(0,3));
                        if (err >= 100)
                          {
                            error_code = err;
                            throw bad_decode(F("received network error: %s")
                                             % errmsg.substr(4));
                          }
                      }
                    catch (boost::bad_lexical_cast)
                      { // ok, so it wasn't a number
                      }
                  }
                throw bad_decode(F("received network error: %s") % errmsg);
              }
            default:
              // ERROR
              return false;
            }
        }
    }
  catch (netsync_error & err)
    {
      W(F("error: %s") % err.msg);
      string const errmsg(lexical_cast<string>(error_code) + " " + err.msg);
      L(FL("queueing 'error' command"));
      netcmd cmd(get_version());
      cmd.write_error_cmd(errmsg);
      write_netcmd(cmd);
      encountered_error = true;
      return true; // Don't terminate until we've send the error_cmd.
    }
}

void
session::request_netsync(protocol_role role,
                         globish const & our_include_pattern,
                         globish const & our_exclude_pattern)
{
  MM(use_transport_auth);
  id nonce2(mk_nonce());
  netcmd request(version);
  rsa_oaep_sha_data hmac_key_encrypted;
  if (use_transport_auth)
    project.db.encrypt_rsa(remote_peer_key_id, nonce2(), hmac_key_encrypted);

  if (use_transport_auth && signing_key.inner()() != "")
    {
      // get our key pair
      load_key_pair(keys, signing_key);

      // make a signature with it;
      // this also ensures our public key is in the database
      rsa_sha1_signature sig;
      keys.make_signature(project.db, signing_key, hello_nonce(), sig);

      request.write_auth_cmd(role, our_include_pattern, our_exclude_pattern,
                             signing_key, hello_nonce,
                             hmac_key_encrypted, sig);
    }
  else
    {
      request.write_anonymous_cmd(role, our_include_pattern, our_exclude_pattern,
                                  hmac_key_encrypted);
    }
  write_netcmd(request);
  set_session_key(nonce2());

  key_identity_info remote_key;
  remote_key.id = remote_peer_key_id;
  if (!remote_key.id.inner()().empty())
    project.complete_key_identity_from_id(keys, app.lua, remote_key);

  wrapped->on_begin(session_id, remote_key);
}

void
session::request_automate()
{
  MM(use_transport_auth);
  id nonce2(mk_nonce());
  rsa_oaep_sha_data hmac_key_encrypted;
  rsa_sha1_signature sig;
  if (use_transport_auth)
    {
      project.db.encrypt_rsa(remote_peer_key_id, nonce2(), hmac_key_encrypted);

      if (signing_key.inner()() != "")
        {
          keys.make_signature(project.db, signing_key, hello_nonce(), sig);
        }
    }

  netcmd request(version);
  request.write_automate_cmd(signing_key, hello_nonce,
                             hmac_key_encrypted, sig);
  write_netcmd(request);
  set_session_key(nonce2());

  key_identity_info remote_key;
  remote_key.id = remote_peer_key_id;
  if (!remote_key.id.inner()().empty())
    project.complete_key_identity_from_id(keys, app.lua, remote_key);

  wrapped->on_begin(session_id, remote_key);
}

void
session::queue_bye_cmd(u8 phase)
{
  L(FL("queueing 'bye' command, phase %d")
    % static_cast<size_t>(phase));
  netcmd cmd(get_version());
  cmd.write_bye_cmd(phase);
  write_netcmd(cmd);
}

bool
session::process_bye_cmd(u8 phase,
                         transaction_guard & guard)
{

// Ideal shutdown
// ~~~~~~~~~~~~~~~
//
//             I/O events                 state transitions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~~~~~~~~~
//                                        client: C_WORKING
//                                        server: S_WORKING
// 0. [refinement, data, deltas, etc.]
//                                        client: C_SHUTDOWN
//                                        (client checkpoints here)
// 1. client -> "bye 0"
// 2.           "bye 0"  -> server
//                                        server: S_SHUTDOWN
//                                        (server checkpoints here)
// 3.           "bye 1"  <- server
// 4. client <- "bye 1"
//                                        client: C_CONFIRMED
// 5. client -> "bye 2"
// 6.           "bye 2"  -> server
//                                        server: S_CONFIRMED
// 7. [server drops connection]
//
//
// Affects of I/O errors or disconnections
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   C_WORKING: report error and fault
//   S_WORKING: report error and recover
//  C_SHUTDOWN: report error and fault
//  S_SHUTDOWN: report success and recover
//              (and warn that client might falsely see error)
// C_CONFIRMED: report success
// S_CONFIRMED: report success

  switch (phase)
    {
    case 0:
      if (voice == server_voice &&
          protocol_state == working_state)
        {
          protocol_state = shutdown_state;
          guard.do_checkpoint();
          queue_bye_cmd(1);
        }
      else
        error(error_codes::bad_command,
              "unexpected bye phase 0 received");
      break;

    case 1:
      if (voice == client_voice &&
          protocol_state == shutdown_state)
        {
          protocol_state = confirmed_state;
          queue_bye_cmd(2);
        }
      else
        error(error_codes::bad_command, "unexpected bye phase 1 received");
      break;

    case 2:
      if (voice == server_voice &&
          protocol_state == shutdown_state)
        {
          protocol_state = confirmed_state;
          return false;
        }
      else
        error(error_codes::bad_command, "unexpected bye phase 2 received");
      break;

    default:
      error(error_codes::bad_command,
            (F("unknown bye phase %d received") % phase).str());
    }

  return true;
}

static
protocol_role
corresponding_role(protocol_role their_role)
{
  switch (their_role)
    {
    case source_role:
      return sink_role;
    case source_and_sink_role:
      return source_and_sink_role;
    case sink_role:
      return source_role;
    }
  I(false);
}

bool session::handle_service_request()
{
  enum { is_netsync, is_automate } is_what;
  bool auth;

  // netsync parameters
  protocol_role role;
  globish their_include;
  globish their_exclude;

  // auth parameters
  key_id client_id;
  id nonce1;
  rsa_sha1_signature sig;

  rsa_oaep_sha_data hmac_encrypted;


  switch (cmd_in.get_cmd_code())
    {
    case anonymous_cmd:
      cmd_in.read_anonymous_cmd(role, their_include, their_exclude,
                                hmac_encrypted);
      L(FL("received 'anonymous' netcmd from client for pattern '%s' excluding '%s' "
           "in %s mode\n")
        % their_include % their_exclude
        % (role == source_and_sink_role ? _("source and sink") :
           (role == source_role ? _("source") : _("sink"))));

      is_what = is_netsync;
      auth = false;
      break;
    case auth_cmd:
      cmd_in.read_auth_cmd(role, their_include, their_exclude,
                           client_id, nonce1, hmac_encrypted, sig);
      L(FL("received 'auth(hmac)' netcmd from client '%s' for pattern '%s' "
           "exclude '%s' in %s mode with nonce1 '%s'\n")
        % client_id % their_include % their_exclude
        % (role == source_and_sink_role ? _("source and sink") :
           (role == source_role ? _("source") : _("sink")))
        % nonce1);
      is_what = is_netsync;
      auth = true;
      break;
    case automate_cmd:
      cmd_in.read_automate_cmd(client_id, nonce1, hmac_encrypted, sig);
      is_what = is_automate;
      auth = true;
      break;
    default:
      I(false);
    }
  set_session_key(hmac_encrypted);

  if (auth && !project.db.public_key_exists(client_id))
    {
      key_name their_name;
      keypair their_pair;
      if (keys.maybe_get_key_pair(client_id, their_name, their_pair))
        {
          project.db.put_key(their_name, their_pair.pub);
        }
      else
        {
          auth = false;
        }
    }
  if (auth)
    {
      if (!(nonce1 == saved_nonce))
        {
          error(error_codes::failed_identification,
                F("detected replay attack in auth netcmd").str());
        }
      if (project.db.check_signature(client_id, nonce1(), sig) != cert_ok)
        {
          error(error_codes::failed_identification,
                F("bad client signature").str());
        }
      authenticated = true;
      remote_peer_key_id = client_id;
    }

  switch (is_what)
    {
    case is_netsync:
      wrapped = make_shared<netsync_session>(this,
                                             app.opts, app.lua,
                                             project,
                                             keys,
                                             corresponding_role(role),
                                             their_include,
                                             their_exclude,
                                             connection_counts::create());
      break;
    case is_automate:
      wrapped = make_shared<automate_session>(app, this, nullptr, nullptr);
      break;
    }

  key_identity_info client_identity;
  if (authenticated)
    {
      client_identity.id = client_id;
      if (!client_identity.id.inner()().empty())
        project.complete_key_identity_from_id(keys, app.lua, client_identity);
    }

  wrapped->on_begin(session_id, client_identity);
  wrapped->prepare_to_confirm(client_identity, use_transport_auth);

  netcmd cmd(get_version());
  cmd.write_confirm_cmd();
  write_netcmd(cmd);


  completed_hello = true;
  authenticated = true;
  return true;
}

void session::write_netcmd(netcmd const & cmd)
{
  if (!encountered_error)
  {
    string buf;
    cmd.write(buf, write_hmac);
    queue_output(buf);
    L(FL("queued outgoing netcmd of type '%d'") % cmd.get_cmd_code());
  }
  else
    L(FL("dropping outgoing netcmd of type '%d' (because we're in error unwind mode)")
      % cmd.get_cmd_code());
}

u8 session::get_version() const
{
  return version;
}

protocol_voice session::get_voice() const
{
  return voice;
}

string session::get_peer() const
{
  return peer;
}

int session::get_error_code() const
{
  return error_code;
}

bool session::get_authenticated() const
{
  return authenticated;
}

void
session::error(int errcode, string const & errmsg)
{
  error_code = errcode;
  throw netsync_error(errmsg);
}

void
session::note_bytes_in(int count)
{
  if (wrapped)
    {
      wrapped->note_bytes_in(count + unnoted_bytes_in);
      unnoted_bytes_in = 0;
    }
  else
    unnoted_bytes_in += count;
}

void
session::note_bytes_out(int count)
{
  if (wrapped)
    {
      wrapped->note_bytes_out(count + unnoted_bytes_out);
      unnoted_bytes_out = 0;
    }
  else
    unnoted_bytes_out += count;
}

// Local Variables:
// mode: C++
// fill-column: 76
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
// vim: et:sw=2:sts=2:ts=2:cino=>2s,{s,\:s,+s,t0,g0,^-2,e-2,n-2,p2s,(0,=s:
