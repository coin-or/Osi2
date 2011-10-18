# Copyright (C) 2010 Lou Hafer
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id$

# Author:  Lou Hafer        SFU        2011-06-30

AUTOMAKE_OPTIONS = foreign

########################################################################
#                               libOsi2                                 #
########################################################################

# Name of the library compiled in this directory.
lib_LTLIBRARIES = libOsi2.la

# List all source files for this library, including headers

libOsi2_la_SOURCES = \
	Osi2API.hpp \
	Osi2ControlAPI.hpp Osi2ControlAPI_Imp.hpp Osi2ControlAPI_Imp.cpp \
	Osi2CtrlAPIMessages.hpp Osi2CtrlAPIMessages.cpp

# This is for libtool
libOsi2_la_LDFLAGS = $(LT_LDFLAGS)

# Make sure the library includes the necessary dependencies
libOsi2_la_LIBADD =

# Here list all include flags.
AM_CPPFLAGS = -I`$(CYGPATH_W) $(srcdir)/../Osi2Plugin` \
	      $(OSI2PLUGIN_CFLAGS) $(OSI2LIB_CFLAGS)
	      

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
	Osi2API.hpp \
	Osi2ControlAPI.hpp \
	Osi2ProbMgmtAPI.hpp

