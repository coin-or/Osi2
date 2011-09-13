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

unitTest_LDADD = ../src/Osi2Plugin/libOsi2Plugin.la

# Now add the include paths for compilation
# Here list all include flags, relative to this "srcdir" directory.  This
# "cygpath" stuff is necessary to compile with native compilers on Windows
AM_CPPFLAGS =\
	-I`$(CYGPATH_W) $(srcdir)/../src/Osi2Plugin` \
	-I`$(CYGPATH_W) $(srcdir)/../src/Osi2` \
	$(CLP_CFLAGS) \
	$(COINUTILS_CFLAGS)

# This line is necessary to allow VPATH compilation with MS compilers
# on Cygwin
DEFAULT_INCLUDES = -I. -I`$(CYGPATH_W) $(srcdir)`

test: unitTest$(EXEEXT)
	./unitTest$(EXEEXT)

.PHONY: test






