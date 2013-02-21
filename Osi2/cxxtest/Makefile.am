# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: Makefile.am 806 2006-06-12 01:47:32Z andreasw $

# Author:  Andreas Waechter           IBM    2006-04-13

AUTOMAKE_OPTIONS = foreign


########################################################################
#                          unitTest  program                               #
########################################################################

# Name of the executable compiled in this directory.  We want it to be
# installed in the 'bin' directory
noinst_PROGRAMS = cxxUnitTest

# List all source files for this executable, including headers
cxxUnitTest_SOURCES = \
  $(srcdir)/Osi2PluginTest.cpp $(srcdir)/Osi2PluginTest.hpp
nodist_cxxUnitTest_SOURCES = cxxUnitTest.cpp 

# The driver will be built by running cxxtestgen
BUILT_SOURCES = cxxUnitTest.cpp
CLEANFILES = cxxUnitTest.cpp
cxxUnitTest.cpp: $(cxxUnitTest_SOURCES) 
	$(CXXTESTGEN) --fog-parser --error-printer \
	    -o cxxUnitTest.cpp $(cxxUnitTest_SOURCES)

# Add the necessary libraries

cxxUnitTest_LDADD = $(CXXUNITTESTDEPS_LIBS)

# Now add the include paths for compilation

AM_CPPFLAGS = $(CXXUNITTESTDEPS_CFLAGS) -DOSI2PLUGINDIR=\"$(libdir)\"

# This line is necessary to allow VPATH compilation with MS compilers
# on Cygwin
DEFAULT_INCLUDES = -I. -I`$(CYGPATH_W) $(srcdir)` \
	-I`$(CYGPATH_W) $(srcdir)/../src/Osi2` \
	-I`$(CYGPATH_W) ../src/Osi2`

test: cxxUnitTest$(EXEEXT)
	./cxxUnitTest$(EXEEXT)

.PHONY: test






