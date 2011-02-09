## test/simlfs sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
nofail_PRG = LFSTestSuite
fail_PRG =

#
check_PROGRAMS = $(nofail_PRG) $(fail_PRG)
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = $(fail_PRG)

#
LFSTestSuite_SOURCES = $(simlfs_test_bin_cc_sources)
LFSTestSuite_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
LFSTestSuite_LDADD =
LFSTestSuite_LDFLAGS =  \
	$(BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/simfqt/core/libsimfqt.la \
	$(top_builddir)/airinv/core/libairinv.la \
	$(top_builddir)/simlfs/core/libsimlfs.la
