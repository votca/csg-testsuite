AM_CPPFLAGS = -I$(top_srcdir)/include 

noinst_PROGRAMS = overlap_integral

overlap_integral_SOURCES = overlap_integral.cc

AM_LDFLAGS =  

LDADD = ../src/libmoo/libmoo.a $(LIBS_VOTCA_TOOLS) $(BOOST_PROGRAM_OPTIONS_LIB) -lm -lpthread
