# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: Makefile.am 806 2006-06-12 01:47:32Z andreasw $

# Author:  Andreas Waechter           IBM    2006-04-13

AUTOMAKE_OPTIONS = foreign

include $(top_srcdir)/BuildTools/Makemain.inc

########################################################################
#                          unitTest  program                           #
########################################################################

# Name of the executable compiled in this directory. Test programs are
# not installed.

noinst_PROGRAMS = cxxUnitTest

# List all source files for this executable, including headers.

cxxUnitTest_SOURCES = Osi2PluginTest.cpp \
		      Osi2PluginTest.hpp

nodist_cxxUnitTest_SOURCES = cxxUnitTest.cpp 

# The driver will be built by running cxxtestgen. For a VPATH build, cxxtestgen
# isn't running in the cxxtest source directory, hence needs some help to
# find the source header files.  The trickery with set -x allows the user
# to see the cxxtestgen command without being bothered by the source file
# prep that precedes it.

BUILT_SOURCES = cxxUnitTest.cpp
CLEANFILES = cxxUnitTest.cpp
cxxUnitTest.cpp: $(cxxUnitTest_SOURCES)
	@for file in $(cxxUnitTest_SOURCES) ; do \
	  if expr "$$file" : '^.*\.h.*$$' &>/dev/null ; then \
	    tmp="$$tmp $(srcdir)/$$file" ; \
	  fi ; \
	done ; \
	set -x ; \
	$(CXXTESTGEN) --fog-parser --error-printer \
	    -o cxxUnitTest.cpp $$tmp

# Add the necessary libraries

cxxUnitTest_LDADD = ../src/Osi2Plugin/libOsi2Plugin.la \
		    ../src/Osi2/libOsi2.la \
		    $(CXXUNITTESTDEPS_LFLAGS)

# Now add the include paths for compilation

AM_CPPFLAGS = -I$(srcdir)/../src/Osi2Plugin \
	      $(CXXUNITTESTDEPS_CFLAGS) -DOSI2PLUGINDIR=\"$(libdir)\"

test: cxxUnitTest$(EXEEXT)
	./cxxUnitTest$(EXEEXT)

.PHONY: test

