
mtn_setup()

get("test.manifest")

get("test.tags")

get("e")

check(mtn("--branch=foo.bar", "cvs_import", "e"), 0, false, false)
check(mtn("--branch=foo.bar", "co"))
check(indir("foo.bar", mtn("automate", "get_manifest_of")), 0, true)
canonicalize("stdout")
check(samefile("test.manifest", "stdout"))
check(indir("foo.bar", mtn("list", "tags")), 0, true)
canonicalize("stdout")
check(samefile("test.tags", "stdout"))