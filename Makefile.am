# Copyright (C) 2003 Graydon Hoare <graydon@pobox.com>
#               2014 Markus Wanner <markus@bluegap.ch>
#
# This program is made available under the GNU GPL version 2.0 or
# greater. See the accompanying file COPYING for details.
#
# This program is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.

# Makefile for monotone

AUTOMAKE_OPTIONS = subdir-objects -Wno-portability
ACLOCAL_AMFLAGS  = -I m4

SUBDIRS = doc

CMD_SOURCES =								\
	src/cmd.hh src/cmd.cc src/cmd_netsync.cc src/cmd_list.cc 	\
	src/cmd_packet.cc src/cmd_key_cert.cc src/cmd_merging.cc 	\
	src/cmd_db.cc src/cmd_diff_log.cc src/cmd_ws_commit.cc 		\
	src/cmd_othervcs.cc src/cmd_automate.cc src/cmd_files.cc	\
	src/cmd_conflicts.cc

SANITY_CORE_SOURCES =							\
	src/sanity.cc src/sanity.hh src/quick_alloc.hh src/vector.hh 	\
	src/base.hh src/simplestring_xform.cc src/simplestring_xform.hh	\
	src/constants.cc src/constants.hh src/numeric_vocab.hh		\
	src/platform.hh src/origin_type.hh

LUAEXT_SOURCES = 							\
	src/vocab.hh src/vocab.cc src/vocab_terms.hh			\
	src/vocab_macros.hh src/vocab_cast.hh src/charset.cc		\
	src/charset.hh src/paths.cc src/paths.hh src/interner.hh	\
	src/vocab_hash.hh src/luaext_mkstemp.cc				\
	src/luaext_parse_basic_io.cc src/luaext_guess_binary.cc		\
	src/luaext_platform.cc src/luaext_globish.cc src/lua.cc		\
	src/lua.hh src/mkstemp.cc src/file_io.cc src/file_io.hh		\
	src/globish.cc src/globish.hh src/basic_io.cc src/basic_io.hh	\
	src/char_classifiers.hh

MOST_SOURCES = 								\
	$(SANITY_CORE_SOURCES) $(LUAEXT_SOURCES)			\
	src/platform-wrapped.hh src/rev_types.hh src/mtn-sanity.cc	\
	src/mtn-sanity.hh src/ui.cc src/ui.hh src/app_state.cc		\
	src/app_state.hh src/automate_ostream.hh			\
	src/automate_ostream_demuxed.hh	src/automate_reader.hh		\
	src/automate_reader.cc src/automate_stdio_helpers.hh		\
	src/botan_pipe_cache.hh	src/cache_logger.hh src/cache_logger.cc	\
	src/commands.cc src/commands.hh $(CMD_SOURCES)			\
	src/colorizer.cc src/colorizer.hh				\
	src/diff_output.cc src/diff_output.hh				\
	src/lua_hooks.cc src/lua_hooks.hh 				\
	src/transforms.cc src/transforms.hh				\
	src/update.cc src/update.hh					\
	src/work.cc src/migrate_work.cc src/work.hh			\
	src/cert.cc src/cert.hh						\
	src/cow_trie.hh							\
	src/project.cc src/project.hh					\
	src/outdated_indicator.cc src/outdated_indicator.hh		\
	src/database.cc src/database.hh					\
	src/key_store.cc src/key_store.hh				\
	src/ssh_agent.cc src/ssh_agent.hh				\
	src/keys.cc src/keys.hh						\
	src/packet.cc src/packet.hh					\
	src/key_packet.cc src/key_packet.hh				\
	src/rcs_file.cc src/rcs_file.hh					\
	src/xdelta.cc src/xdelta.hh					\
	src/migration.hh src/migrate_schema.cc src/migrate_ancestry.cc	\
	src/refiner.cc src/refiner.hh					\
	src/enumerator.cc src/enumerator.hh				\
	src/netsync.hh src/netsync.cc            			\
	src/network/automate_session.hh src/network/automate_session.cc	\
	src/network/connection_info.hh src/network/connection_info.cc	\
	src/network/listener_base.hh src/network/listener_base.cc	\
	src/network/listener.hh src/network/listener.cc			\
	src/network/make_server.hh src/network/make_server.cc		\
	src/network/netsync_session.hh src/network/netsync_session.cc	\
	src/network/reactable.hh src/network/reactable.cc		\
	src/network/reactor.hh src/network/reactor.cc			\
	src/network/session.hh src/network/session.cc			\
	src/network/session_base.hh src/network/session_base.cc		\
	src/network/wrapped_session.hh src/network/wrapped_session.cc	\
	src/netxx_pipe.cc src/netxx_pipe.hh				\
	src/netcmd.cc src/netcmd.hh					\
	src/merkle_tree.cc src/merkle_tree.hh 				\
	src/lcs.cc src/lcs.hh						\
	src/rcs_import.cc src/rcs_import.hh				\
	src/git_change.cc src/git_change.hh src/git_export.cc		\
	src/git_export.hh						\
	src/revision.cc src/ancestry.cc src/revision.hh			\
	src/cset.cc src/cset.hh						\
	src/roster.cc src/roster.hh 					\
	src/mt_version.cc src/mt_version.hh				\
	src/automate.cc							\
	src/database_check.cc						\
	src/epoch.cc src/epoch.hh					\
	src/gzip.cc src/gzip.hh						\
	src/inodeprint.cc src/inodeprint.hh				\
	src/selectors.cc src/selectors.hh				\
	src/annotate.cc src/annotate.hh					\
	src/restrictions.cc src/restrictions.hh				\
	src/hmac.cc src/hmac.hh						\
	src/string_queue.hh						\
	src/maybe_workspace_updater.cc src/maybe_workspace_updater.hh	\
	src/merge_roster.cc src/merge_conflict.cc src/merge_roster.hh	\
	src/merge_content.cc src/merge_3way.cc src/merge_content.hh	\
	src/legacy.cc src/legacy.hh src/uri.cc src/uri.hh		\
	src/graph.cc src/graph.hh					\
	src/roster_delta.cc src/roster_delta.hh				\
	src/sha1.cc							\
	src/pcrewrap.cc src/pcrewrap.hh					\
	src/rev_height.cc src/rev_height.hh				\
	src/rev_output.cc src/rev_output.hh				\
	src/asciik.cc src/asciik.hh					\
	src/dates.cc src/dates.hh src/date_format.hh			\
									\
	src/lru_writeback_cache.hh src/hybrid_map.hh src/lazy_rng.hh	\
									\
	src/cleanup.hh src/cycle_detector.hh src/adler32.hh		\
	src/netio.hh src/gettext.h					\
	src/option.cc src/option.hh src/options.cc src/options.hh	\
	src/options_list.hh src/options_applicator.hh			\
	src/options_applicator.cc src/option_reset_info.hh		\
	src/specialized_lexical_cast.cc src/lexical_cast.hh		\
	src/parallel_iter.hh src/safe_map.hh src/pch.hh			\
	src/current_exception.hh

NETXX_SOURCES =								\
	src/netxx/accept.cxx src/netxx/accept.h src/netxx/address.cxx	\
	src/netxx/common.h src/netxx/compat.h src/netxx/datagram.cxx	\
	src/netxx/datagramserver.cxx src/netxx/osutil.cxx		\
	src/netxx/osutil.h src/netxx/peer.cxx src/netxx/probe.cxx 	\
	src/netxx/probe_impl.h src/netxx/probe_select.cxx 		\
	src/netxx/recvfrom.cxx src/netxx/recvfrom.h src/netxx/resolve.h	\
	src/netxx/serverbase.cxx src/netxx/serverbase.h			\
	src/netxx/sockaddr.cxx src/netxx/sockaddr.h			\
	src/netxx/socket.cxx src/netxx/socket.h src/netxx/sockopt.cxx 	\
	src/netxx/stream.cxx src/netxx/streambase.cxx 			\
	src/netxx/streamserver.cxx src/netxx/address.h 			\
	src/netxx/datagram.h src/netxx/datagramserver.h 		\
	src/netxx/netbuf.h src/netxx/netxx.h src/netxx/peer.h 		\
	src/netxx/probe.h src/netxx/probeinfo.h src/netxx/sockopt.h 	\
	src/netxx/stream.h src/netxx/streambase.h 			\
	src/netxx/streamserver.h src/netxx/timeout.h src/netxx/types.h

UNIX_PLATFORM_SOURCES =							\
	src/unix/read_password.cc src/unix/get_system_flavour.cc	\
	src/unix/process.cc src/unix/terminal.cc src/unix/inodeprint.cc	\
	src/unix/fs.cc src/unix/make_io_binary.cc			\
	src/unix/os_strerror.cc src/unix/cputime.cc 			\
	src/unix/ssh_agent_platform.cc src/unix/ssh_agent_platform.hh	\
	src/unix/parse_date.cc

WIN32_PLATFORM_SOURCES =						\
	src/win32/read_password.cc src/win32/get_system_flavour.cc	\
	src/win32/process.cc src/win32/terminal.cc			\
	src/win32/inodeprint.cc	src/win32/fs.cc 			\
	src/win32/make_io_binary.cc src/win32/os_strerror.cc		\
	src/win32/cputime.cc src/win32/ssh_agent_platform.cc		\
	src/win32/ssh_agent_platform.hh src/win32/parse_date.cc

# these files (part of the main program) contain code subject to unit testing

UNIT_TESTEES =								\
	src/basic_io.cc src/charset.cc src/commands.cc src/cset.cc 	\
	src/git_change.cc src/globish.cc src/graph.cc src/merge_3way.cc	\
	src/merge_roster.cc src/mkstemp.cc src/netcmd.cc 		\
	src/netxx_pipe.cc src/option.cc src/outdated_indicator.cc	\
	src/pcrewrap.cc src/refiner.cc src/restrictions.cc		\
	src/revision.cc src/simplestring_xform.cc src/transforms.cc 	\
	src/uri.cc src/vocab.cc src/xdelta.cc

# these files contain the actual unit tests
# yes, we have to list them all explicitly, with directory prefix, or
# automake gets very confused
UNIT_TESTERS =								\
	test/unit/tests/basic_io.cc test/unit/tests/charset.cc		\
	test/unit/tests/commands.cc test/unit/tests/cset.cc		\
	test/unit/tests/git_change.cc test/unit/tests/globish.cc	\
	test/unit/tests/graph.cc test/unit/tests/merge_3way.cc		\
	test/unit/tests/merge_roster.cc test/unit/tests/mkstemp.cc	\
	test/unit/tests/netcmd.cc test/unit/tests/netxx_pipe.cc		\
	test/unit/tests/option.cc test/unit/tests/outdated_indicator.cc	\
	test/unit/tests/pcrewrap.cc test/unit/tests/refiner.cc 		\
	test/unit/tests/restrictions.cc test/unit/tests/revision.cc	\
	test/unit/tests/simplestring_xform.cc				\
	test/unit/tests/transforms.cc					\
	test/unit/tests/uri.cc test/unit/tests/vocab.cc			\
	test/unit/tests/xdelta.cc test/unit/tests/numeric_vocab.cc	\
	test/unit/tests/sha1.cc test/unit/tests/string_queue.cc		\
	test/unit/unit_tester.cc

# these files contain unit tests that need access to private interfaces
# of the code under test, so they #include the code under test.  thus,
# to avoid multiple-definition errors, for these files the code under
# test is not part of UNIT_TESTEES.

UNIT_TESTERS_PRIV =						\
	test/unit/tests/dates.cc test/unit/tests/key_packet.cc 	\
	test/unit/tests/packet.cc test/unit/tests/paths.cc 	\
	test/unit/tests/rev_height.cc test/unit/tests/roster.cc

# these files do not contain unit tests, but are required for unit testing
# note that some are and some aren't part of the main program
UNIT_TEST_SUPPORT =						\
	src/constants.cc src/file_io.cc src/gzip.cc src/hmac.cc \
	src/lcs.cc src/merkle_tree.cc src/roster_delta.cc 	\
	src/sanity.cc src/specialized_lexical_cast.cc		\
	test/unit/randomfile.hh test/unit/unit_tests.hh		\
	test/unit/randomizer.hh test/unit/randomizer.cc		\
	test/unit/roster_tests.hh

# primaries

bin_PROGRAMS = mtn
mtn_SOURCES = $(MOST_SOURCES) src/monotone.cc
nodist_mtn_SOURCES = src/std_hooks.cc src/schema.cc src/package_revision.cc src/package_full_revision.cc

noinst_PROGRAMS = util/txt2c
util_txt2c_SOURCES = util/txt2c.cc

EXTRA_PROGRAMS =

bin_SCRIPTS = mtnopt
dist_bin_SCRIPTS = extra/bin/mtn-cleanup
noinst_SCRIPTS = extra/building/dump-test-logs.sh

check_PROGRAMS = test/bin/unit_tester test/bin/tester test/bin/check_net
test_bin_unit_tester_SOURCES = $(UNIT_TESTERS) $(UNIT_TESTERS_PRIV) \
	$(UNIT_TESTEES) $(UNIT_TEST_SUPPORT)
test_bin_tester_SOURCES = test/src/tester.cc test/src/tester-plaf.hh src/transforms.cc src/gzip.cc \
	$(SANITY_CORE_SOURCES) $(LUAEXT_SOURCES) src/pcrewrap.cc \
	src/specialized_lexical_cast.cc
nodist_test_bin_tester_SOURCES = test/src/testlib.cc

noinst_LIBRARIES = src/libplatform.a src/lib3rdparty.a
src_libplatform_a_SOURCES = src/platform.hh
src_lib3rdparty_a_SOURCES = $(NETXX_SOURCES)

bashcompdir = $(sysconfdir)/bash_completion.d
bashcomp_DATA = extra/shell/monotone.bash_completion

monotonedir = $(datadir)/monotone

mtnhooksdir = $(monotonedir)/hooks
dist_mtnhooks_DATA = \
	extra/mtn-hooks/authorize_remote_automate.lua			\
	extra/mtn-hooks/get_passphrase_from_file.lua			\
	extra/mtn-hooks/monotone-buildbot.lua				\
	extra/mtn-hooks/monotone-ciabot.lua				\
	extra/mtn-hooks/monotone-cluster-push.lua			\
	extra/mtn-hooks/monotone-cvs-ignore.lua				\
	extra/mtn-hooks/monotone-mail-notify.lua

mtnscriptsdir = $(monotonedir)/scripts
dist_mtnscripts_SCRIPTS = \
	extra/mtn-hooks/monotone-ciabot.py				\
	extra/mtn-hooks/monotone-mail-notify

# Some of the smaller programs do not have --help or --version,
# make them exempt of the check for those.  This specially applies
# to programs intended to be called by mtn hooks, for which these
# options aren't necessary.
# NOTE: in the expansion of this variable, each filename MUST be
# surrounded by one space before and after.  Having a tab between
# two file names will guarantee that they won't be exempt.  This
# is really a small but serious bug in the automake templates.
# backslash+newline+whitespace will be reinterpreted as whitespace
# by make.
AM_INSTALLCHECK_STD_OPTIONS_EXEMPT = \
	extra/bin/mtn-cleanup extra/mtn-hooks/monotone-ciabot.py \
	extra/mtn-hooks/monotone-mail-notify

# flags

# sets DEFS for just this object (autoconf's -DHAVE_CONFIG_H is useless)
# the variable prevents this from overriding automake's unix/fs.o rule
DEFS =
UNIX=src/unix/
$(UNIX)fs.$(OBJEXT) : DEFS += -DLOCALEDIR="\"$(localedir)\""

if BUILD_PCH

PCH_SOURCE = $(srcdir)/pch.hh
# the .gch file only will be seen if it's in the same dir as the .hh file,
# so need to copy it for separate build directories
PCH_BUILD = pch-build.hh
PCH_FLAGS = -Winvalid-pch -include $(PCH_BUILD)
PCH_FILE = $(PCH_BUILD).gch
sinclude $(PCH_FILE).dep

*.o *.lo *.obj: $(PCH_FILE)

$(PCH_BUILD): $(PCH_SOURCE)
	cp $(PCH_SOURCE) $(PCH_BUILD)

$(PCH_FILE): $(PCH_BUILD)
	$(MAKE) PCH_FLAGS= pch-file

pch-file:
	$(CXXCOMPILE) -x c++-header $(PCH_BUILD) -o $(PCH_FILE) \
	-MT $(PCH_FILE)  -MD -MP -MF "./$(PCH_FILE).Tdep" \
	&& mv "./$(PCH_FILE).Tdep" "./$(PCH_FILE).dep" \
	|| rm "./$(PCH_FILE).Tdep"
endif
AM_CXXFLAGS = $(PCH_FLAGS)

SUFFIXES = .gch .merged.po .gmo

AM_CPPFLAGS = $(intl_CFLAGS) $(pcre_CFLAGS) $(botan_CFLAGS)	\
	      $(lua_CFLAGS) $(sqlite3_CFLAGS) $(idn_CFLAGS)

LDADD = src/libplatform.a src/lib3rdparty.a $(intl_LIBS) $(pcre_LIBS)	\
	$(botan_LIBS) $(lua_LIBS) $(sqlite3_LIBS) $(idn_LIBS)

if WIN32_PLATFORM
  src_libplatform_a_SOURCES += $(WIN32_PLATFORM_SOURCES)
  mtn_SOURCES += src/win32/main.cc
  test_bin_tester_SOURCES += test/src/win32/tester-plaf.cc
  test_bin_check_net_SOURCES = test/src/win32/tester-check-net.cc
  AM_CPPFLAGS += -DWIN32
  LDADD += -lshfolder -lws2_32 -liconv -liphlpapi
else
  src_libplatform_a_SOURCES += $(UNIX_PLATFORM_SOURCES)
  mtn_SOURCES += src/unix/main.cc
  test_bin_tester_SOURCES += test/src/unix/tester-plaf.cc
  test_bin_check_net_SOURCES = test/src/unix/tester-check-net.cc
endif

if MISSING_INET_PTON
  src_lib3rdparty_a_SOURCES += src/netxx/inet_pton.cxx src/netxx/inet_pton.h
  AM_CPPFLAGS += -DNETXX_NO_PTON
endif

if MISSING_INET_NTOP
  src_lib3rdparty_a_SOURCES += src/netxx/inet_ntop.cxx src/netxx/inet_ntop.h
  AM_CPPFLAGS += -DNETXX_NO_NTOP
endif

if MISSING_GETADDRINFO
  src_lib3rdparty_a_SOURCES += src/netxx/resolve_gethostbyname.cxx \
			       src/netxx/resolve_getservbyname.cxx
else
  src_lib3rdparty_a_SOURCES += src/netxx/resolve_getaddrinfo.cxx
endif

if MISSING_INET6
  AM_CPPFLAGS += -DNETXX_NO_INET6
endif

# extra files

contribdir = $(docdir)/contrib
contrib_prefix = contrib
contrib_data = $(shell cd $(srcdir); find $(contrib_prefix) | sed -e 's,^\./,,')

examplesdir = $(docdir)/examples
examples_prefix = examples
examples_data =								\
	examples/000README.txt						\
	examples/mirror.rc examples/push.rc examples/read-permissions	\
	examples/write-permissions					\
									\
	examples/server-setup/README examples/server-setup/serverrc	\
	examples/server-setup/monotone.init				\
	examples/server-setup/monotone.run				\
									\
	examples/user-setup/README examples/user-setup/monotonerc

EXTRA_DIST =								\
	HACKING INSTALL INSTALL_windows_cygwin.txt			\
	INSTALL_windows_mingw.txt INSTALL_windows_msvc.txt UPGRADE	\
	util/audit-includes util/do-editor-vars.sh			\
	$(wildcard $(srcdir)/m4/*.m4)					\
									\
	src/schema.sql src/unix/README src/std_hooks.lua		\
	src/util/mtnopt.in						\
									\
	test/common							\
	test/func test/func-testsuite.lua				\
	test/tester test/tester-testsuite.lua				\
	$(wildcard $(srcdir)/test/unit/*.cc)				\
	$(wildcard $(srcdir)/test/unit/*.hh)				\
	$(wildcard $(srcdir)/test/unit/tests/*.cc)			\
	test/unit-testsuite.lua						\
	test/extra test/extra-testsuite.lua				\
	test/src/testlib.lua						\
									\
	src/package_revision.txt src/package_full_revision_dist.txt	\
									\
	extra/README extra/building/dump-test-logs.sh.in		\
	extra/shell/monotone.bash_completion.in				\
	extra/shell/monotone_gen_bash_completion_table.pl		\
									\
	innosetup/README.txt						\
	innosetup/monotone.iss.in innosetup/modpath.iss 		\
	innosetup/monotone.bmp innosetup/dlls.sh			\
									\
	mac/readme-mac.txt mac/package-intro.rtf			\
	mac/monotone.pmproj mac/monotone-pkg-logo.gif			\
									\
	cygport/README.txt						\
	cygport/monotone.cygwin.patch					\
	cygport/monotone.cygport					\
									\
	notes/release-checklist.txt					\
	notes/README.testing notes/tester.txt notes/testsuite.txt	\
									\
	visualc/monotone.sln						\
	visualc/monotone.vcproj visualc/botan.vcproj visualc/lua.vcproj	\
	visualc/netxx.vcproj visualc/pcre.vcproj visualc/sqlite3.vcproj	\
	visualc/stringprep.vcproj visualc/txt2c.vcproj			\
	visualc/tester.vcproj visualc/unit_tester.vcproj		\
	visualc/runtests.bat						\
	visualc/config.h visualc/mt-stdint.h visualc/stdint.h		\
	visualc/unistd.h						\
	visualc/unknown_package.txt					\
									\
	$(contrib_data) $(examples_data)

# Message translation support
# INST_LINGUAS is set by configure, based on the po/LINGUAS file and
# the user's LINGUAS env. variable.  Additional settings are in
# po/Makevars.
#
# Only the foo.po-update target will overwrite a file in the source
# po/ directory.

include $(srcdir)/po/Makevars
ALL_POFILES   := $(wildcard $(srcdir)/po/*.po)
ALL_LINGUAS   := $(basename $(notdir $(ALL_POFILES)))
ALL_POREADMES := $(addprefix po/README.,$(ALL_LINGUAS))
ALL_GMOFILES  := $(addprefix po/,$(addsuffix .gmo, $(ALL_LINGUAS)))
CLEAN_POFILES := $(addprefix po/,$(addsuffix .merged.po, $(ALL_LINGUAS))) \
		 po/$(PACKAGE).pot
DISTCLEAN_GMOFILES:= $(ALL_GMOFILES)

EXTRA_DIST += $(ALL_POFILES) $(ALL_POREADMES) po/LINGUAS po/README

.PHONY: all-nls install-nls uninstall-nls
if USE_NLS
# for installation
GMOFILES := $(addprefix po/,$(addsuffix .gmo, $(INST_LINGUAS)))

all-nls: $(GMOFILES)

install-nls: all-nls
	@linguas='$(INST_LINGUAS)'; set -e; for lang in $$linguas; do \
	  dir=$(DESTDIR)$(localedir)/$$lang/LC_MESSAGES; \
	  echo " $(MKDIR_P) '$$dir'"; \
	  $(MKDIR_P) $$dir; \
	  echo " $(INSTALL_DATA) po/$$lang.gmo '$$dir/$(PACKAGE).mo'"; \
	  $(INSTALL_DATA) po/$$lang.gmo $$dir/$(PACKAGE).mo; \
	done

uninstall-nls:
	@linguas='$(INST_LINGUAS)'; set -e; for lang in $$linguas; do \
	  echo " rm -f '$(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$(PACKAGE).mo'"; \
	  rm -f $(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$(PACKAGE).mo; \
	  echo " rmdir '$(DESTDIR)$(localedir)/$$lang/LC_MESSAGES'; \
	  rmdir $(DESTDIR)$(localedir)/$$lang/LC_MESSAGES || :; \
	  echo " rmdir '$(DESTDIR)$(localedir)/$$lang'; \
	  rmdir $(DESTDIR)$(localedir)/$$lang || :; \
	done
else
# We need to provide these targets because all-local, install-data-local
# and uninstall-local expects them as dependencies, further down.
all-nls:
	@:
install-nls:
	@:
uninstall-nls:
	@:
endif

.PHONY: dist-nls
if REBUILD_NLS
po/%.gmo: po/%.merged.po
	$(V_msgfmt)$(MSGFMT) -c --statistics -o $@ $<

po/%.merged.po: $(srcdir)/po/%.po po/$(PACKAGE).pot
	$(V_msgmerge)$(MSGMERGE) $^ -q -o $@

# for translators' use
UPDATEPOFILES = $(addprefix po/,$(addsuffix .po-update, $(ALL_LINGUAS)))

po/%.po-update: $(srcdir)/po/%.po po/$(PACKAGE).pot
	$(MSGMERGE) --update $^

update-po: $(UPDATEPOFILES)

update-gmo: update-po $(ALL_GMOFILES)

# for the template file
POTFILES = $(filter-out package_%, $(MOST_SOURCES)) \
	$(UNIX_PLATFORM_SOURCES) $(WIN32_PLATFORM_SOURCES) \
	src/monotone.cc src/std_hooks.lua

po/$(PACKAGE).pot: $(POTFILES)
	[ -d po ] || mkdir -p po
	$(V_xgettext)$(XGETTEXT) -o$@.in -D. -D$(top_srcdir) -cTRANSLATORS: \
	  --copyright-holder='$(COPYRIGHT_HOLDER)' \
	  --msgid-bugs-address='$(PACKAGE_BUGREPORT)' \
	  $(XGETTEXT_PKG_OPTS) $(XGETTEXT_OPTS) \
	  $(POTFILES)
        YEAR=$(word 1, $(subst -, ,$(BUILD_DATE)))
	sed -e 's/FIRST AUTHOR <EMAIL@ADDRESS>, YEAR./Please see po\/README.<lang> for individual copyrights./' \
	  -e 's/(C) YEAR/(C) 2003 - $(YEAR)/' -e 's/PACKAGE/$(PACKAGE)/' \
	  -e 's/SOME DESCRIPTIVE TITLE/monotone message string catalogue/' < $@.in > $@
	rm $@.in

dist-nls: $(ALL_GMOFILES)
	cp $(ALL_GMOFILES) $(distdir)/po
else

po/%.gmo: $(srcdir)/po/%.gmo
#	Some versions of gmake just drop the argument, but still
#	execute the rule. Therefore, we check if the source is not
#	empty.
	[[ -z $< ]] || cp $< $@

dist-nls:
	@echo "*** Cannot 'make dist' without xgettext" >&2; exit 1

endif

_install_data = \
	list='$(1)'; test -n "$(3)" || list=; \
	for p in $$list; do \
	  if test -f "$$p" || test -d "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  src="$$d$$p"; \
	  dest="`if [ "$$p" != "$(2)" ]; then echo $$p | sed -e 's,$(2)/,,'; fi`"; \
	  if test -d "$$src"; then \
	    echo " $(MKDIR_P) '$(DESTDIR)$(3)/$$dest'"; \
	    $(MKDIR_P) "$(DESTDIR)$(3)/$$dest" || exit $$?; \
	  else \
	    destd="`dirname $$dest`"; \
	    if ! test -d "$(DESTDIR)$(3)/$$destd"; then \
	      echo " $(MKDIR_P) '$(DESTDIR)$(3)/$$destd'"; \
	      $(MKDIR_P) "$(DESTDIR)$(3)/$$destd" || exit $$?; \
	    fi; \
	    echo " $(INSTALL_DATA) $$src '$(DESTDIR)$(3)/$$dest'"; \
	    $(INSTALL_DATA) $$src "$(DESTDIR)$(3)/$$dest" || exit $$?; \
	  fi; \
	done
_uninstall_data = \
	list='$(1)'; test -n "$(3)" || list=; \
	dirs_rev=; \
	for p in $$list; do \
	  to_rm="`if [ "$$p" != "$(2)" ]; then echo $$p | sed -e 's,$(2)/,,'; fi`"; \
	  if [ -n "$$to_rm" ]; then \
	    if test -d "$(DESTDIR)$(3)/$$to_rm"; then \
	      dirs_rev="echo '$$to_rm';$${dirs_rev}"; \
	    else \
	      echo " rm -f '$(DESTDIR)$(3)/$$to_rm'"; \
	      rm -f $$src "$(DESTDIR)$(3)/$$to_rm" || exit $$?; \
	    fi; \
	  fi; \
	done; \
	eval "$$dirs_rev" | while read to_rmdir; do \
	  echo " rmdir '$(DESTDIR)$(3)/$$to_rmdir'"; \
	  rmdir "$(DESTDIR)$(3)/$$to_rmdir" || exit $$?; \
	done

install-contrib-data: $(contrib_data)
	@$(call _install_data,$(contrib_data),$(contrib_prefix),$(contribdir))
uninstall-contrib-data:
	@$(call _uninstall_data,$(contrib_data),$(contrib_prefix),$(contribdir))

install-examples-data: $(examples_data)
	@$(call _install_data,$(examples_data),$(examples_prefix),$(examplesdir))
uninstall-examples-data:
	@$(call _uninstall_data,$(examples_data),$(examples_prefix),$(examplesdir))

# Another attempt at working around abs_topbuilddir not being defined
# or not absolute.
abs_topbuilddir=$(shell cd $(top_builddir); pwd)

# build an OS X installer
# requires sudo since packagemaker just looks at directory permissions.
PKG_INST_ROOT = $(abs_topbuilddir)/pkg_inst_root
monotone-$(PACKAGE_VERSION).pkg: doc/monotone.pdf doc/html
	sudo rm -rf $(PKG_INST_ROOT)
	rm -rf "$@"
	$(MAKE) install-strip DESTDIR=$(PKG_INST_ROOT)
	mkdir -p $(PKG_INST_ROOT)/Developer/Documentation/monotone
	cp doc/monotone.pdf $(PKG_INST_ROOT)/Developer/Documentation/monotone/
	cp -r doc/html $(PKG_INST_ROOT)/Developer/Documentation/monotone/html
	plutil -convert xml1 -o monotone-xml.pmproj.in $(srcdir)/mac/monotone.pmproj
	sed "s/__MONOTONE_VERSION__/$(PACKAGE_VERSION)/g" \
		< monotone-xml.pmproj.in > monotone-xml.pmproj
	cp $(srcdir)/mac/monotone-pkg-logo.gif $(srcdir)/mac/package-intro.rtf .
	cp $(srcdir)/COPYING COPYING.txt
	chmod -R o-w,g-w,a+rX $(PKG_INST_ROOT)
	sudo chown -R root:wheel $(PKG_INST_ROOT)/usr
	sudo chown -R root:admin $(PKG_INST_ROOT)/Developer
	/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker \
		-build -proj monotone-xml.pmproj -p "$@"

# build an OS X distribution disk image
dmg: monotone-$(PACKAGE_VERSION).dmg

monotone-$(PACKAGE_VERSION).dmg: monotone-$(PACKAGE_VERSION).pkg
	rm -rf $(top_builddir)/dmg_root "$@"
	mkdir $(top_builddir)/dmg_root
	cp -R $^ $(top_builddir)/dmg_root/
	chmod -R a+rX $(top_builddir)/dmg_root
	hdiutil create -srcfolder "$(top_builddir)/dmg_root" \
		"$@" -format UDZO -scrub -imagekey zlib-level=9 \
		-volname monotone-$(PACKAGE_VERSION)

# build win32 installer using INNO setup
ISCC=@ISCC@

win32-installer: monotone-$(PACKAGE_VERSION)-setup.exe

monotone-$(PACKAGE_VERSION)-setup.exe: mtn-stripped.exe html info $(ALL_GMOFILES) linguas.iss dlls.iss monotone.iss
	$(ISCC) monotone.iss

mtn-stripped.exe: mtn.exe
	strip $< -o $@

dlls.iss: mtn-stripped.exe ${top_srcdir}/Makefile.am
	rm -f $@
	for DLL in $$( ${top_srcdir}/innosetup/dlls.sh mtn-stripped.exe | xargs cmd //c echo) ; do \
	    echo "Source: \"$$DLL\"; DestDir: \"{app}\"" >> $@ ; \
	done

linguas.iss: ${top_srcdir}/Makefile.am
	rm -rf $@
	for LANG in $(INST_LINGUAS) ; do \
		echo "Source: \"{#BUILDDIR}\\po\\$${LANG}.gmo\"; DestDir: \"{app}\\locale\\$${LANG}\\LC_MESSAGES\"; DestName: \"monotone.mo\"; Flags: ignoreversion" >> $@ ; \
	done


# testsuite stuff (could this possibly be more ugly?)  To get
# parallelism, we cannot use Automake's TESTS feature at all.  The
# shell script embedded in the 'check-testers' rule is partially
# borrowed from automake 1.9's check.am

.PHONY: check-testers
check-testers: test/tester.status test/unit.status test/func.status test/extra.status
	@all=0; failed=0; error=0;					       \
	for f in $^; do							       \
	  all=`expr $$all + 1`;						       \
	  if test -f $$f; then						       \
	    s=`cat $$f`;						       \
	    if test "$$s" = 0; then					       \
	      :;							       \
	    elif test "$$s" = 1; then					       \
	      failed=`expr $$failed + 1`;				       \
	    else							       \
	      error=`expr $$error + 1`;					       \
	    fi;								       \
	  else								       \
	    error=`expr $$error + 1`;					       \
	  fi;								       \
	done;								       \
	if test "$$failed" = 0 && test "$$error" = 0; then		       \
	  exit=0;							       \
	  report="";							       \
	  banner="All $$all test suites passed";			       \
	else								       \
	  exit=1;							       \
	  report="Please report to $(PACKAGE_BUGREPORT)";		       \
	  if test "$$error" = 0; then					       \
	    banner="$$failed of $$all test suites failed";		       \
	  elif test "$$failed" = 0; then				       \
	    banner="$$error of $$all test suites malfunctioned";	       \
	  else								       \
	    banner="$$failed of $$all test suites failed"		       \
	    banner="$$banner and $$error malfunctioned";		       \
	  fi;								       \
	fi;								       \
	rl=`echo $$report | wc -c`;					       \
	bl=`echo $$banner | wc -c`;					       \
	if test $$rl -gt $$bl; then					       \
	  dashes=`echo "$$report" | sed s/./=/g`;			       \
	else								       \
	  dashes=`echo "$$banner" | sed s/./=/g`;			       \
	fi;								       \
	echo $$dashes;							       \
	echo $$banner;							       \
	test -z "$$report" || echo "$$report";				       \
	exit $$exit

run_%_tests: Makefile
	$(AM_V_GEN){ echo '#!/bin/sh';							  	\
	  echo 'export LANG=C';								  	\
	  echo 'PATH="$(abs_topbuilddir):$(abs_topbuilddir)/test/bin:$$PATH"';	  		\
	  echo '$(top_builddir)/test/bin/tester "$(srcdir)/test/$*-testsuite.lua" "$$@"';	\
	  echo 'echo $$? > $(top_builddir)/test/$*.status';			  	  	\
	  echo 'exit 0'; } > $@ && \
	chmod 755 $@

# The leading + causes Make to treat this as a recursive invocation,
# allowing it to participate in the jobserver protocol.
test/%.status: test/%-testsuite.lua test/bin/tester$(EXEEXT) FORCE
	+$(AM_V_at)LANG=C PATH="$(abs_topbuilddir):$(abs_topbuilddir)/test/bin:$$PATH"	\
		"$(top_builddir)/test/bin/tester"			\
		"$(srcdir)/test/$*-testsuite.lua";			\
		echo $$? > $@
test/unit.status : test/bin/unit_tester$(EXEEXT)
test/func.status : mtn$(EXEEXT) test/bin/check_net$(EXEEXT)

# Creates the needed artifacts for manual test execution
.PHONY: testers
testers: mtn$(EXEEXT) $(check_PROGRAMS) \
	run_tester_tests run_unit_tests run_func_tests run_extra_tests

# We want the tests re-run even if the .status files already exist.
# .PHONY does not work for that (bad interaction with pattern rules),
# but the FORCE hack does.
.PHONY: FORCE
FORCE:
.PRECIOUS: run_tester_tests run_unit_tests run_func_tests run_extra_tests

# FIXME: if it annoys anybody, we could clean dvi, pdf and info files
#        only in a separate maintainer-clean target as suggested by
#	 automake's man page
.PHONY: mostlyclean-tests
mostlyclean-tests:
	rm -rf test/work
	@ls test/unit/tests/*/__driver__.lua 2>/dev/null | while read d; do \
	  echo " rm -f '$$d'"; \
	  rm -f $$d; \
	  echo " rmdir '$${d%/*}'"; \
	  rmdir $${d%/*} || :; \
	done

# With the MacOS X toolchain, some stuff may be generated in the form of subdirs
.PHONY: mostlyclean-util
mostlyclean-util:
	rm -rf util/txt2c.dSYM

MOSTLYCLEANFILES = mtn.1 \
	run_tester_tests   run_func_tests   run_unit_tests   run_extra_tests \
	test/tester.status test/func.status test/unit.status test/extra.status

# we generate some source files to copy data into the executable
# note that the only things that should go in BUILT_SRCS are things
# that need to be generated early on 'make all'; this is _not_ true of
# generated source files, but it is true for the PCH if used.

# package_revision.txt and package_full_revision_dist.txt are shipped
# in the distribution, so don't delete them here
CLEAN_SRCS = $(nodist_mtn_SOURCES) $(nodist_test_bin_tester_SOURCES)	\
	src/package_revision.cc src/package_full_revision.cc		\
	src/package_revision_raw.txt					\
	src/package_full_revision.txt src/package_full_revision_raw.txt

BUILT_SOURCES = $(PCH_FILE) $(PCH_BUILD)

CLEANFILES = $(bin_SCRIPTS) $(noinst_SCRIPTS) $(bashcomp_DATA) \
	$(CLEAN_SRCS) $(CLEAN_POFILES)

DISTCLEANFILES = src/xgettext.opts src/pch-build.hh.gch.dep		\
	$(DISTCLEAN_GMOFILES)

# a maintainer might still want to get rid of src/package_revision.txt and
# src/package_full_revision_dist.txt to rebuild them.

MAINTAINERCLEANFILES = \
	src/package_revision.txt src/package_full_revision_dist.txt

# automake provides no nice way to build a helper program to execute
# on the build machine, so we need our own rule.  almost all the
# standard flags variables are inappropriate.

util/txt2c$(EXEEXT): util/txt2c.cc
	$(V_bcxx)$(CXX_FOR_BUILD) $(CXXFLAGS) -o $@ $^

# files generated using txt2c
src/std_hooks.cc: src/std_hooks.lua util/txt2c$(EXEEXT)
	$(V_txt2c)./util/txt2c std_hooks $< $@

src/schema.cc: src/schema.sql util/txt2c$(EXEEXT)
	$(V_txt2c)./util/txt2c schema $< $@

src/package_revision.cc: src/package_revision.txt util/txt2c$(EXEEXT)
	$(V_txt2c)./util/txt2c --strip-trailing package_revision $< $@

src/package_full_revision.cc: src/package_full_revision.txt util/txt2c$(EXEEXT)
	$(V_txt2c)./util/txt2c package_full_revision $< $@

test/src/testlib.cc: test/src/testlib.lua util/txt2c$(EXEEXT)
	$(V_txt2c)./util/txt2c testlib $< $@

# Support for scripts and extras
do_subst = sed -e 's,[@]PACKAGE_VERSION[@],$(PACKAGE_VERSION),' \
	-e 's,[@]sysconfdir[@],$(sysconfdir),'

mtnopt: src/util/mtnopt.in Makefile
	$(V_subst)$(do_subst) < $< > $@ && chmod +x $@

extra/shell/monotone.bash_completion: extra/shell/monotone.bash_completion.in \
	$(bin_PROGRAMS) extra/shell/monotone_gen_bash_completion_table.pl \
	Makefile
	$(MKDIR_P) extra/shell
	( sed -e '/^[@]generated-command-list[@]$$/,$$d' < $<; \
	  MTN=./mtn$(EXEEXT) \
	    perl $(srcdir)/extra/shell/monotone_gen_bash_completion_table.pl; \
	  sed -e '1,/^[@]generated-command-list[@]$$/d' < $< ) | \
	  $(do_subst) > $@

extra/building/dump-test-logs.sh: extra/building/dump-test-logs.sh.in Makefile
	$(MKDIR_P) extra/building
	$(V_subst)$(do_subst) < $< > $@ && chmod +x $@

# This is phony, so that we always try to rebuild it.  If it succeeds
# in calculating changes, it produces its target; otherwise, its
# target does not exist.
.PHONY: src/package_revision_raw.txt
# if both attempts fail, then we need to remove the empty file that >
# creates, and also make sure that the shell command exits
# successfully; the rm -f ensures both
src/package_revision_raw.txt:
	$(AM_V_GEN)REAL_BLDDIR=$(abs_topbuilddir); \
	(cd $(srcdir) && $$REAL_BLDDIR/mtn --root=. automate get_base_revision_id) 2>/dev/null >$@ \
	|| (cd $(srcdir) && mtn --root=. automate get_base_revision_id) 2>/dev/null >$@ \
	|| rm -f $@
src/package_revision.txt: src/package_revision_raw.txt
	$(AM_V_GEN)set -e; if [ -f $< ]; then \
	  cp $< $@; \
	fi; \
	if [ ! -f $@ -a -f $(srcdir)/$@ ]; then \
	  cp $(srcdir)/$@ $@; \
	fi; \
	[ -f $@ ] || echo "unknown" > $@

# To avoid a dependency loop here, what we do is:
#           calculate changes
#                  |
#      package_full_revision_raw.txt
#              |                \  <-- add "generated from cached data" text
#              |      package_full_revision_dist.txt
#              |           /
#          package_full_revision.txt

# See above comment.
.PHONY: src/package_full_revision_raw.txt
src/package_full_revision_raw.txt:
	$(AM_V_GEN)REAL_BLDDIR=$(abs_topbuilddir); \
	(cd $(srcdir) && $$REAL_BLDDIR/mtn --root=. automate get_current_revision) 2>/dev/null >$@ \
	|| (cd $(srcdir) && mtn --root=. automate get_current_revision) 2>/dev/null >$@ \
	|| rm -f $@
# See above comment -- the file this depends on may or may not exist.
# This file, however, must always exist, so that 'make dist' can
# include it in the tarball it generates.  So there are three cases:
#   -- if the raw file does not exist, then
#      -- if the dist file exists, then we leave it alone
#      -- if the dist file does not exist, then we say "unknown"
#   -- if the raw file does exist, then we regenerate the dist file
src/package_full_revision_dist.txt: src/package_full_revision_raw.txt
	$(AM_V_GEN)set -e; if [ -f $< ]; then \
	  cp -f $< $@ \
	  && (echo ''; \
	      echo '  Generated from data cached in the distribution;'; \
	      echo '  further changes may have been made.') >> $@; \
	fi; \
	if [ ! -f $@ -a -f $(srcdir)/$@ ]; then \
	  cp $(srcdir)/$@ $@; \
	fi; \
	[ -f $@ ] || echo "unknown" > $@
# The raw file may not exist, but the dist file definitely does; we
# just take the first dependency that exists.
src/package_full_revision.txt: src/package_full_revision_raw.txt src/package_full_revision_dist.txt
	$(AM_V_GEN)rm -f $@; \
	for SRC in $^; do \
	    ([ -f $$SRC -a ! -f $@ ] && cp -f $$SRC $@) || true; \
	done

# This is a magic directive copy-and-pasted, then modified, from the
# automake 1.9 manual, section 13.4, "Checking the distribution".
# Normally, 'distcheck' does a clean build, and then afterwards runs
# 'distclean', and 'distclean' is supposed to remove everything that
# the build created.  However, we have some targets
# (package_revision.txt, package_full_revision_dist.txt) that we
# distribute, but then always attempt to rebuild optimistically, and
# then if that fails fall back on the distributed versions.  This
# means that 'distclean' should _not_ remove those files, since they
# are distributed, yet building the package will generate those files,
# thus automake thinks that 'distclean' _should_ remove those files,
# and 'distcheck' gets cranky if we don't.  So basically what this
# line does is tell 'distcheck' to shut up and ignore those two files.
distcleancheck_listfiles = find . -type f -a ! -name package_revision.txt -a ! -name package_full_revision_dist.txt

# the distcheck-base.hh checks for errors in the use of base.hh.
# It's being used by distcheck-hook further down.
.PHONY: distcheck-base.hh
distcheck-base.hh:
	cd $(srcdir) && $(SHELL) util/audit-includes \
		$(sort $(mtn_SOURCES) $(test_bin_unit_tester_SOURCES) $(test_bin_tester_SOURCES) \
                       $(UNIX_PLATFORM_SOURCES) $(WIN32_PLATFORM_SOURCES) \
                       src/win32/main.cc src/unix/main.cc)

man1_MANS = mtn.1
dist_man1_MANS = src/util/mtnopt.1 extra/bin/mtn-cleanup.1

# All local variants of automake-supported targets here, depending on all
# the things we want to do.  This is to support multiple things to do in
# each of them, as dependencies.
all-local: all-nls
install-data-local: install-nls \
	install-contrib-data install-examples-data
uninstall-local: uninstall-nls \
	uninstall-contrib-data uninstall-examples-data
mostlyclean-local: mostlyclean-tests mostlyclean-util
check-local: check-testers

# All hooks supported by automake here, depending on all the things we want
# to hook in.  This is to support multiple things into the same hook, as
# dependencies.
distcheck-hook: distcheck-base.hh
dist-hook: dist-nls
# Kill off any backup files that got pulled in by one of the
# recursive subdirectory includes in EXTRA_DIST
# note use of slightly non-portable "-delete" action
	find $(distdir) \( -name '*.bak' -o -name '*.orig' -o -name '*.rej' \
	    -o -name '*~' -o -name '#*#' -o -name '*.swp' \) -delete

mtn.1: mtn$(EXEEXT)
	$(AM_V_GEN)REAL_BLDDIR=$(abs_topbuilddir); \
	(cd $(srcdir) && $$REAL_BLDDIR/mtn manpage --norc) 2>/dev/null >$@ || rm -f $@

doc/monotone.dvi: dvi
doc/monotone.pdf: pdf
doc/monotone.ps: ps
doc/html doc/monotone.html: html

# Not really phony, but the only way to make sure we wander into
# the subdirectory and do the build with dependencies and all.
.PHONY: doc/monotone.dvi doc/monotone.pdf doc/monotone.ps doc/monotone.html \
	doc/html

# Verbosity goo.
V_bcxx       = $(V_bcxx_$(V))
V_bcxx_      = $(V_bcxx_$(AM_DEFAULT_VERBOSITY))
V_bcxx_0     = @echo "  BCXX  " $@;

V_txt2c      = $(V_txt2c_$(V))
V_txt2c_     = $(V_txt2c_$(AM_DEFAULT_VERBOSITY))
V_txt2c_0    = @echo "  TXT2C " $@;

V_subst      = $(V_subst_$(V))
V_subst_     = $(V_subst_$(AM_DEFAULT_VERBOSITY))
V_subst_0    = @echo "  SUBST " $@;

V_xgettext   = $(V_xgettext_$(V))
V_xgettext_  = $(V_xgettext_$(AM_DEFAULT_VERBOSITY))
V_xgettext_0 = @echo "XGETTEXT" $@;

V_msgfmt     = $(V_msgfmt_$(V))
V_msgfmt_    = $(V_msgfmt_$(AM_DEFAULT_VERBOSITY))
V_msgfmt_0   = @echo "  MSGFMT" $@;

V_msgmerge   = $(V_msgmerge_$(V))
V_msgmerge_  = $(V_msgmerge_$(AM_DEFAULT_VERBOSITY))
V_msgmerge_0 = @echo "MSGMERGE" $@;

V_makeinfo   = $(V_makeinfo_$(V))
V_makeinfo_  = $(V_makeinfo_$(AM_DEFAULT_VERBOSITY))
V_makeinfo_0 = @echo "MAKEINFO" $@;
