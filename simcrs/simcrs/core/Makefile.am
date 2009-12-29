# core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libsimcrs.la

libsimcrs_la_SOURCES = $(service_h_sources) $(service_cc_sources)
libsimcrs_la_LIBADD = \
	$(top_builddir)/simcrs/basic/libbas.la \
	$(top_builddir)/simcrs/bom/libbom.la \
	$(top_builddir)/simcrs/factory/libfac.la \
	$(top_builddir)/simcrs/command/libcmd.la \
	$(top_builddir)/simcrs/service/libsvc.la
libsimcrs_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) \
	$(top_builddir)/stdair/core/libstdair.la \
	$(top_builddir)/airsched/core/libairsched.la \
	-version-info $(GENERIC_LIBRARY_VERSION)

# Header files
#pkginclude_HEADERS = $(service_h_sources)
#nobase_nodist_pkginclude_HEADERS = $(top_builddir)/simcrs/config.h

