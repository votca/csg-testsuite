AM_CPPFLAGS = $(BOOST_CPPFLAGS) $(CFLAGS_XML) $(GSL_CFLAGS) -I$(top_srcdir)/include 

noinst_PROGRAMS = basic property_test spline_test version_test

basic_SOURCES = basic.cc

property_test_SOURCES = property_test.cc

spline_test_SOURCES = spline_test.cc

version_test_SOURCES = version_test.cc

AM_LDFLAGS = 

LDADD = ../src/libtools/libvotca_tools.a $(GSL_LIBS) $(LIBS_XML) -lpthread 
