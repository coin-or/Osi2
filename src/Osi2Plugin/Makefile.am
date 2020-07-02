# Copyright (C) 2010 Lou Hafer
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id$

# Author:  Lou Hafer        SFU        2010-07-29

AUTOMAKE_OPTIONS = foreign

include $(top_srcdir)/BuildTools/Makemain.inc

########################################################################
#                               libOsi2                                 #
########################################################################

# Name of the library compiled in this directory.

lib_LTLIBRARIES = libOsi2Plugin.la

# List all source files for this library, including headers.

libOsi2Plugin_la_SOURCES = \
	Osi2DynamicLibrary.hpp Osi2DynamicLibrary.cpp \
	Osi2Plugin.hpp \
	Osi2PluginManager.cpp Osi2PluginManager.hpp \
	Osi2PlugMgrMessages.cpp Osi2PlugMgrMessages.hpp

# This is for libtool.

libOsi2Plugin_la_LDFLAGS = $(LT_LDFLAGS)

# We need the dynamic link library.

libOsi2Plugin_la_LIBADD = -ldl

# Here list all include flags.

AM_CPPFLAGS = -DOSI2_BUILD \
	      -I$(srcdir)/../Osi2 \
	      $(OSI2PLUGIN_CFLAGS) -DOSI2PLUGINDIR=\"$(libdir)\"

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in pkgincludedir.

includecoindir = $(pkgincludedir)
includecoin_HEADERS = \
	Osi2Plugin.hpp \
	Osi2PluginManager.hpp

