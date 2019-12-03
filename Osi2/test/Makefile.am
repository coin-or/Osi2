# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: Makefile.am 806 2006-06-12 01:47:32Z andreasw $

# Author:  Andreas Waechter           IBM    2006-04-13

AUTOMAKE_OPTIONS = foreign

include $(top_srcdir)/BuildTools/Makemain.inc

########################################################################
#                          unitTest  program                               #
########################################################################

# Name of the executable compiled in this directory.  We want it to be
# installed in the 'bin' directory.

noinst_PROGRAMS = unitTest

# List all source files for this executable, including headers.

unitTest_SOURCES = unitTest.cpp 

# Add the necessary libraries

unitTest_LDADD = ../src/Osi2Plugin/libOsi2Plugin.la \
		 ../src/Osi2/libOsi2.la \
		 $(UNITTESTDEPS_LFLAGS)

# Now add the include paths for compilation

AM_CPPFLAGS = -DOSI2_BUILD \
	      -I$(srcdir)/../src/Osi2 \
	      -I$(srcdir)/../src/Osi2Plugin \
	      -I$(srcdir)/../src/Osi2Shims \
	      $(UNITTESTDEPS_CFLAGS) \
	      -DOSI2UTSAMPLEDIR=\"@SAMPLE_DATA@\" \
	      -DOSI2UTNETLIBDIR=\"@NETLIB_DATA@\"

test: unitTest$(EXEEXT)
	./unitTest$(EXEEXT)

.PHONY: test

