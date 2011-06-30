# Copyright (C) 2010 Lou Hafer
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id$

# Author:  Lou Hafer        SFU        2010-07-29

AUTOMAKE_OPTIONS = foreign

########################################################################
#                               libOsi2                                 #
########################################################################

# Name of the library compiled in this directory.  We don't want it
# installed just yet.
lib_LTLIBRARIES = libOsi2ClpShim.la

# List all source files for this library, including headers

libOsi2ClpShim_la_SOURCES = \
	Osi2ClpShim.cpp Osi2ClpShim.hpp

# This is for libtool
libOsi2ClpShim_la_LDFLAGS = $(LT_LDFLAGS) -module

# Make sure the dynamic library includes the necessary dependencies
libOsi2ClpShim_la_LIBADD = $(libdir)/libOsi2Plugin.la

# Here list all include flags.
AM_CPPFLAGS = $(COINUTILS_CFLAGS) $(CLP_CFLAGS) -I$(srcdir)/../Osi2Plugin

# This line is necessary to allow VPATH compilation.
# This "cygpath" stuff is necessary to compile with native compilers on Windows.
DEFAULT_INCLUDES = -I. -I`$(CYGPATH_W) $(srcdir)`

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in 'includedir/coin'

includecoindir = $(includedir)/coin
includecoin_HEADERS = \
	Osi2ClpShim.hpp

