AM_CPPFLAGS = -I$(top_srcdir)/include  $(CFLAGS_XML) 

noinst_PROGRAMS = overlap_integral

overlap_integral_SOURCES = overlap_integral.cc

AM_LDFLAGS =  

LDADD = -lmoo $(LIBS_VOTCA_TOOLS) $(BOOST_PROGRAM_OPTIONS_LIB) $(LIBS_XML) -lm -lpthread
