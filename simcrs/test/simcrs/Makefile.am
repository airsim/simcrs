## test/simcrs sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
check_PROGRAMS = CRSTestSuite
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

CRSTestSuite_SOURCES = CRSTestSuite.hpp CRSTestSuite.cpp
CRSTestSuite_CXXFLAGS= $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS)
CRSTestSuite_LDADD =
CRSTestSuite_LDFLAGS = \
		$(BOOST_LIBS) $(CPPUNIT_LIBS) \
		$(top_builddir)/extracppunit/libextracppunit.la \
		$(top_builddir)/stdair/core/libstdair.la \
		$(top_builddir)/simcrs/core/libsimcrs.la
#		$(top_builddir)/airinv/core/libairinv.la

