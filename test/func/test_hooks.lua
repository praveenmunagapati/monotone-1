-- Copyright (C) 2006 Timothy Brownawell <tbrownaw@gmail.com>
--
-- This program is made available under the GNU GPL version 2.0 or
-- greater. See the accompanying file COPYING for details.
--
-- This program is distributed WITHOUT ANY WARRANTY; without even the
-- implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
-- PURPOSE.

-- this is the "testing" set of lua hooks for monotone
-- it's intended to support self-tests, not for use in
-- production. just defines some of the std hooks.

function get_passphrase(keyid)
	return keyid.given_name
end

function accept_testresult_change(old_results, new_results)
   for test,res in pairs(old_results)
   do
      if res == true and new_results[test] ~= true
      then
	 return false
      end
   end
   return true
end

function persist_phrase_ok()
	return true
end

function get_author(branchname)
	return "tester@test.net"
end

function get_date_format_spec()
        return ""
end

function ignore_file(name)
	if (string.find(name, "ts-std", 1, true)) then return true end
	if (string.find(name, "testsuite.log")) then return true end
	if (string.find(name, "test_hooks.lua")) then return true end
	if (string.find(name, "keys")) then return true end
	return false
end

function merge2(left_path, right_path, merged_path, left, right)
	io.write("running merge2 hook\n") 
	return left
     end

merge3 = nil
edit_comment = nil

if (attr_functions == nil) then
  attr_functions = {}
end
attr_functions["test:test_attr"] =
  function(filename, value)
    io.write(string.format("test:test_attr:%s:%s\n", filename, value))
  end

-- Ensure that the mtn:execute attr is set the same for all platforms,
-- since attrs are part of the manifest, and we need revids to be
-- invariant across platforms
if (attr_init_functions == nil) then
  attr_init_functions = {}
end
attr_init_functions["mtn:execute"] =
   function(filename)
        return nil
   end

function get_mtn_command(host)
   return os.getenv("mtn")
end

function get_encloser_pattern(name)
   return "^[[:alnum:]$_]"
end

do
   local std_get_netsync_connect_command
      = get_netsync_connect_command
   function get_netsync_connect_command(uri, args)
      local argv = std_get_netsync_connect_command(uri, args)

      if argv and argv[1] then
	 table.insert(argv, "--confdir="..get_confdir())
	 table.insert(argv, "--rcfile="..get_confdir().."/test_hooks.lua")
	 local x = io.open("custom_test_hooks.lua", "rb")
	 if (x ~= nil) then
	    table.insert(argv, "--rcfile="..get_confdir().."/custom_test_hooks.lua")
	    x:close()
	 end
      end

      return argv;
   end
end
