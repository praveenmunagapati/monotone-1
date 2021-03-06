-- Copyright (c) 2007, 2011 by Richard Levitte <richard@levitte.org>
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions
-- are met:
--
-- 1. Redistributions of source code must retain the above copyright
--    notice, this list of conditions and the following disclaimer.
--
-- 2. Redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in the
--    documentation and/or other materials provided with the distribution.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
-- ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
-- LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
-- A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
-- OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
-- SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
-- LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
-- DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
-- THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
-- (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
-- OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

-------------------------------------------------------------------------------
-- Usage:
--
--    NOTE: THIS SOFTWARE IS ONLY MEANT FOR SERVER PROCESSES!
--    Anything else will fail miserably!
--
--    in your server's monotonerc, add the following include:
--
--	include("/PATH/TO/monotone-cluster-push.lua")
--
--    You may want to set up the following variables:
--
--	MCP_rcfile
--			The absolute path to the configuration file used
--			by this script.
--			Default: {confdir}/cluster-push.rc
--
--    The configuration file must contain basic-io stanzas with the
--    following keys:
--
--    pattern		Branch pattern, as in read-permissions.  It MUST
--			come first in each stanza.
--    server		Server address or address:port to which this
--			pattern should be pushed.  There may be many of
--			these in each stanza.
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- Local hack of the note_netsync_* functions
-------------------------------------------------------------------------------
do
   -- initialise rcfile with a default, or use MCP_rcfile if it's set
   local rcfile = get_confdir() .. "/cluster-push.rc"
   if MCP_rcfile then rcfile = MCP_rcfile end

   local debug = false
   if MCP_debug then debug = true end

   local verbose_startup = false
   if MCP_verbose_startup then verbose_startup = true end

   local branches = {}

   -- returns false if the rcfile wasn't parseable, otherwise a table of
   -- patterns with the servers they each should be pushed to.
   local process_rcfile =
      function (_hook_name, _rcfile)
	 if debug then
	    io.stderr:write(_hook_name, ": reading ", _rcfile,
			    "\n")
	 end
	 local rcfile = io.open(_rcfile, "r")
	 if (rcfile == nil) then
	    io.stderr:write("file ", _rcfile, " cannot be opened\n")
	    return nil
	 end
	 local dat = rcfile:read("*a")
	 io.close(rcfile)
	 if debug then
	    io.stderr:write(_hook_name, ": got this:\n", dat, "\n")
	 end
	 local res = parse_basic_io(dat)
	 if res == nil then
	    io.stderr:write("file ", _rcfile, " cannot be parsed\n")
	    return nil
	 end

	 local pattern_servers = {}
	 local patterns = {}
	 local previous_name = ""
	 for i, item in pairs(res) do
	    if item.name == "pattern" then
	       if debug then
		  io.stderr:write(_hook_name, ": found ",
				  item.name, " = \"", item.values[1],
				  "\"\n")
	       end
	       if previous_name ~= "pattern" then
		  if debug then
		     io.stderr:write(_hook_name, ": clearing patterns because previous_name = \"", previous_name, "\"\n")
		  end
		  patterns = {}
	       end
	       patterns[item.values[1]] = true
	    elseif item.name == "server" then
	       if debug then
		  io.stderr:write(_hook_name, ": found ", item.name, " = \"",
				  item.values[1], "\"\n")
	       end
	       local server = item.values[1]
	       for pattern, b in pairs(patterns) do
		  if b then
		     if not pattern_servers[pattern] then
			pattern_servers[pattern] = {}
		     end
		     table.insert(pattern_servers[pattern], server)
		  end
	       end
	    end
	    previous_name = item.name
	 end
	 return pattern_servers
      end

   local notifier = {
      startup =
	 function(...)
	    local pattern_branches =
	       process_rcfile("monotone-cluster-push: startup", rcfile)
	    if pattern_branches then
	       for pattern, servers in pairs(pattern_branches) do
		  for _,server in pairs(servers) do
		     if not verbose_startup then
			io.stderr:write("monotone-cluster-push: startup: ",
					"pushing pattern \"", pattern,
					"\" to server ", server, "\n")
		     end
		     server_request_sync("push",
					 "mtn://"..server.."?"..pattern,
					 "", "")
		  end
	       end
	    end
	    return "continue",nil
	 end,

      start =
	 function (nonce, ...)
	    if debug then
	       io.stderr:write("monotone-cluster-push: start: ",
			       "initialise branches\n")
	    end
	    branches[nonce] = {}
	    return "continue",nil
	 end,

      cert_received =
	 function (rev_id, key, name, value, nonce, ...)
	    if debug then
	       io.stderr:write("note_netsync_cert_received: cert ", name,
			       " with value ", value, " received\n")
	    end
	    if name == "branch" then
	       if debug then
		  io.stderr:write("monotone-cluster-push: cert_received: ",
				  "branch ", value, " identified\n")
	       end
	       branches[nonce][value] = true
	    end
	    return "continue",nil
	 end,

      revision_received =
	 function (new_id, revision, certs, nonce, ...)
	    for _, item in pairs(certs)
	    do
	       if debug then
		  io.stderr:write("monotone-cluster-push: revision_received: ",
				  "cert ", item.name, " with value ",
				  item.value, " received\n")
	       end
	       if item.name == "branch" then
		  if debug then
		     io.stderr:write("monotone-cluster-push: revision_received: ",
				     "branch ", item.value, " identified\n")
		  end
		  branches[nonce][item.value] = true
	       end
	    end
	    return "continue",nil
	 end,

      ["end"] =
	 function (nonce, status,
		   bytes_in, bytes_out,
		   certs_in, certs_out,
		   revs_in, revs_out,
		   keys_in, keys_out,
		   ...)
	    if debug then
	       io.stderr:write("monotone-cluster-push: end: ",
			       string.format("%d certs, %d revs, %d keys",
					     certs_in, revs_in, keys_in),
			       "\n")
	    end
	    if certs_in > 0 or revs_in > 0 or keys_in > 0 then
	       local pattern_branches =
		  process_rcfile("monotone-cluster-push: end", rcfile, nil)
	       if pattern_branches then
		  for pattern, servers in pairs(pattern_branches) do
		     for branch, _ in pairs(branches[nonce]) do
			if debug then
			   io.stderr:write("monotone-cluster-push: end: ",
					   "trying pattern ", pattern,
					   " with branch ", branch, "\n")
			end
			if globish.match(pattern, branch) then
			   if debug then
			      io.stderr:write("monotone-cluster-push: end: ",
					      "it matches\n")
			   end
			   for _,server in pairs(servers) do
			      io.stderr:write("monotone-cluster-push: end: ",
					      "pushing pattern \"", pattern,
					      "\" to server ", server, "\n")
			      server_request_sync("push",
						  "mtn://"..server.."?"..pattern,
						  "", "")
			   end
			end
		     end
		  end
	       end
	    end
	    return "continue",nil
	 end
   }

   push_netsync_notifier(notifier)
end
