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
noinst_PROGRAMS = unitTest

# List all source files for this executable, including headers
unitTest_SOURCES = unitTest.cpp 

# Add the necessary libraries

unitTest_LDADD = $(UNITTESTDEPS_LIBS)

# Now add the include paths for compilation

AM_CPPFLAGS = $(UNITTESTDEPS_CFLAGS)

# This line is necessary to allow VPATH compilation with MS compilers
# on Cygwin
DEFAULT_INCLUDES = -I. -I`$(CYGPATH_W) $(srcdir)` \
	-I`$(CYGPATH_W) $(srcdir)/../src/Osi2` \
	-I`$(CYGPATH_W) ../src/Osi2`

test: unitTest$(EXEEXT)
	./unitTest$(EXEEXT)

.PHONY: test






