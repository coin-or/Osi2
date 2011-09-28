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
# installed since it will be collected into libOsi2.
lib_LTLIBRARIES = libOsi2Plugin.la

# List all source files for this library, including headers
# As a first cut, to avoid having to translate directory traversal and file
# stat from APR to native, try cutting out this code:
# Osi2Directory.cpp Osi2Directory.hpp
# Osi2Path.cpp Osi2Path.hpp

libOsi2Plugin_la_SOURCES = \
	Osi2DynamicLibrary.hpp Osi2DynamicLibrary.cpp \
	Osi2Plugin.hpp \
	Osi2PluginManager.cpp Osi2PluginManager.hpp \
	Osi2PlugMgrMessages.cpp Osi2PlugMgrMessages.hpp

# This is for libtool
libOsi2Plugin_la_LDFLAGS = $(LT_LDFLAGS)

# And we need the dynamic link library.
libOsi2Plugin_la_LIBADD = -ldl

# Here list all include flags.
AM_CPPFLAGS = $(COINUTILS_CFLAGS) -DOSI2PLUGINDIR=\"$(libdir)\"

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
	Osi2Plugin.hpp \
	Osi2PluginManager.hpp

