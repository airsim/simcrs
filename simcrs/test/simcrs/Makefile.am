## test/simcrs sub-directory
include $(top_srcdir)/Makefile.common

MAINTAINERCLEANFILES = Makefile.in

##
SUBDIRS =

EXTRA_DIST =

##
check_PROGRAMS = sell
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = #

#InventoryTestSuite_SOURCES = InventoryTestSuite.hpp \
				InventoryTestSuite.cpp
#InventoryTestSuite_CXXFLAGS= $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS)
#InventoryTestSuite_LDADD = $(top_builddir)/test/com/libcppunitcore.la
#InventoryTestSuite_LDFLAGS = \
#		$(BOOST_LIBS) $(CPPUNIT_LIBS) \
#		$(top_builddir)/stdair/core/libstdair.la \
#		$(top_builddir)/simcrs/libsimcrs.la

sell_SOURCES = sell.cpp
sell_CXXFLAGS= $(CPPUNIT_CFLAGS) $(BOOST_CFLAGS)
sell_LDADD =
sell_LDFLAGS = $(BOOST_LIBS) \
		$(top_builddir)/stdair/core/libstdair.la \
		$(top_builddir)/simcrs/core/libsimcrs.la
