-- Test showing and setting all possible conflict resolutions in a
-- conflict file. Also test 'conflict show_remaining'.
--
-- Except for dropped/modified; see
-- ../resolve_conflicts_dropped_modified/__driver__.lua

mtn_setup()
get("merge3_hook.lua")

-- Generate a conflicts file, with one conflict for each type of
-- resolution (not necessarily all types of conflict). The list of
-- currently supported resolutions is in ../../merge_conflict.cc, syms
-- declaration; any sym that starts with "resolved".
--
-- resolution               file
-- -- --                    -- --
-- resolved_drop_left       checkout_left.sh abe, gui_left.sh abe
-- resolved_drop_right      checkout_right.sh beth, gui_right.sh abe
-- resolved_keep_left       gui_left.sh beth
-- resolved_keep_right      gui_right.sh beth
-- resolved_internal        simple_file
-- resolved_rename_left     thermostat.c -> thermostat-westinghouse.c
-- resolved_rename_right    thermostat.c -> thermostat-honeywell.c
-- resolved_user_left       user_file (single file content conflict)
-- resolved_user_left       interactive_file (single file content conflict)
-- resolved_user_left       checkout_left.sh beth (two file duplicate name conflict)
-- resolved_user_right      checkout_right.sh abe (two file duplicate name conflict)
--
-- We can't set 'resolved_internal' directly; it is set by 'conflicts store'.

addfile("simple_file", "simple\none\ntwo\nthree\n")
addfile("user_file", "blah blah blah")
addfile("interactive_file", "interactive base")
commit()
base = base_revision()

-- abe creates duplicate name conflicts
addfile("checkout_left.sh", "checkout_left.sh abe 1")
addfile("checkout_right.sh", "checkout_right.sh abe 1")
addfile("gui_left.sh", "gui_left.sh abe 1")
addfile("gui_right.sh", "gui_right.sh abe 1")
addfile("thermostat.c", "thermostat westinghouse")

-- abe creates content conflicts
writefile("interactive_file", "interactive_file abe 1")
writefile("simple_file", "simple\nzero\none\ntwo\nthree\n")
writefile("user_file", "user_file abe 1")
commit("testbranch", "abe_1")
abe_1 = base_revision()

revert_to(base)

-- beth creates duplicate name conflicts
addfile("checkout_left.sh", "checkout_left.sh beth 1")
addfile("checkout_right.sh", "checkout_right.sh beth 1")
addfile("gui_left.sh", "gui_left.sh beth 1")
addfile("gui_right.sh", "gui_right.sh beth 1")
addfile("thermostat.c", "thermostat honeywell")

-- beth creates content conflicts
writefile("interactive_file", "interactive_file beth 1")
writefile("simple_file", "simple\none\ntwo\nthree\nfour\n")
writefile("user_file", "user_file beth 1")
commit("testbranch", "beth_1")
beth_1 = base_revision()

-- Test non-default conflicts file name
mkdir("resolutions")
check (mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "store", abe_1, beth_1), 0, nil, true)
check(samelines("stderr", {"mtn: 8 conflicts with supported resolutions.",
                           "mtn: stored in '_MTN/conflicts-1'"}))
check(samefilestd("conflicts-1", "_MTN/conflicts-1"))

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "show_remaining"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("show_remaining-checkout_left", "stderr"))

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "show_first"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("show_first-checkout_left", "stderr"))

writefile("resolutions/checkout_left.sh", "checkout_left.sh beth 2")
check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first_left", "drop"), 0, nil, nil)
check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first_right", "user", "resolutions/checkout_left.sh"), 0, nil, nil)

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "show_first"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("show_first-checkout_right", "stderr"))

writefile("resolutions/checkout_right.sh", "checkout_right.sh beth 2")
check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first_right", "drop"), 0, nil, nil)
check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first_left", "user", "resolutions/checkout_right.sh"), 0, nil, nil)

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "show_remaining"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("show_remaining-gui_left", "stderr"))

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "show_first"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("show_first-gui_left", "stderr"))

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first_left", "drop"), 0, nil, nil)
check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first_right", "keep"), 0, nil, nil)

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "show_first"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("show_first-gui_right", "stderr"))

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first_left", "keep"), 0, nil, nil)
check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first_right", "drop"), 0, nil, nil)

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "show_remaining"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("show_remaining-thermostat", "stderr"))

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "show_first"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("show_first-thermostat", "stderr"))

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first_left", "rename", "thermostat-westinghouse.c"), 0, nil, nil)
check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first_right", "rename", "thermostat-honeywell.c"), 0, nil, nil)

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "show_first"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("show_first-interactive", "stderr"))

mkdir("_MTN/resolutions")
check(mtn("--rcfile=merge3_hook.lua", "conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first", "interactive", "_MTN/resolutions/interactive_file"), 0, nil, true)
check(samelines("stderr", { "mtn: lua: running merge3 hook",
                            "mtn: interactive merge result saved in '_MTN/resolutions/interactive_file'"}))

check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "show_first"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("show_first-user", "stderr"))

writefile("resolutions/user_file", "user_file merged")
check(mtn("conflicts", "--conflicts-file=_MTN/conflicts-1", "resolve_first", "user", "resolutions/user_file"), 0, nil, nil)

check(samefilestd("conflicts-resolved", "_MTN/conflicts-1"))

-- This succeeds. In 'mtn conflicts store' above, we specified 'abe_1,
-- beth_1', which is the opposite order that 'merge' would choose, so
-- we need to use 'explicit_merge' here.
check(mtn("explicit_merge", "--resolve-conflicts-file", "_MTN/conflicts-1", abe_1, beth_1, "testbranch"), 0, nil, true)
canonicalize("stderr")
check(samefilestd("merge-1", "stderr"))

-- Verify user specified resolution files
check(mtn("update"), 0, nil, false)

check("checkout_left.sh beth 2" == readfile("checkout_left.sh"))
check("checkout_right.sh beth 2" == readfile("checkout_right.sh"))
check("user_file merged" == readfile("user_file"))
check("interactive_file merged" == readfile("interactive_file"))

-- end of file
