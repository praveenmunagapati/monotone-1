-- Import a simple RCS file and verify that all its metadata came
-- through correctly.

expected_file_hash = "774be3106edbb6d80be36dbb548d62401dcfa0fe"
expected_rev_hash = "f31b5f6dfeddd51ebf19cd44c4176c68bff51709"

mtn_setup()
check(get("cvs-repository"))
check(get("expected_log_output"))

check(mtn("--branch=testbranch", "cvs_import", "cvs-repository/test"),
      0, false, false)

check(mtn("automate", "get_file", expected_file_hash), 0, false)
check(mtn("log", "-r", expected_rev_hash, "--no-graph"), 0, true, false)
canonicalize ("stdout")
check(samefile("stdout", "expected_log_output"))
