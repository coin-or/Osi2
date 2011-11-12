# Copyright (C) 2010 Lou Hafer
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id$

# Author:  Lou Hafer        SFU        2010-07-29

AUTOMAKE_OPTIONS = foreign

# Name of the libraries compiled in this directory.  We don't want it
# installed just yet.
lib_LTLIBRARIES = libOsi2ClpShim.la libOsi2ClpHeavyShim.la \
		  libOsi2GlpkHeavyShim.la

########################################################################
#                      libOsi2ClpShim, ClpHeavyShim                    #
########################################################################

# List all source files for this library, including headers

#	Osi2Osi1API_Clp.cpp Osi2Osi1API_Clp.hpp

libOsi2ClpShim_la_SOURCES = \
	Osi2ProbMgmtAPI_Clp.cpp Osi2ProbMgmtAPI_Clp.hpp \
	Osi2ClpShim.cpp Osi2ClpShim.hpp

libOsi2ClpHeavyShim_la_SOURCES = \
	Osi2ProbMgmtAPI_ClpHeavy.cpp Osi2ProbMgmtAPI_ClpHeavy.hpp \
	Osi2Osi1API_ClpHeavy.cpp Osi2Osi1API_ClpHeavy.hpp \
	Osi2ClpHeavyShim.cpp Osi2ClpHeavyShim.hpp

# This is for libtool
libOsi2ClpShim_la_LDFLAGS = $(LT_LDFLAGS) -module
libOsi2ClpHeavyShim_la_LDFLAGS = $(LT_LDFLAGS) -module

# Make sure the dynamic library includes the necessary dependencies
libOsi2ClpShim_la_LIBADD = $(OSI2CLPSHIM_LIBS)
libOsi2ClpHeavyShim_la_LIBADD = $(OSI2CLPHEAVYSHIM_LIBS)

########################################################################
#                      libOsi2GlpkHeavyShim                            #
########################################################################

if COIN_HAS_OSIGLPK
libOsi2GlpkHeavyShim_la_SOURCES = \
	Osi2Osi1API_GlpkHeavy.cpp Osi2Osi1API_GlpkHeavy.hpp \
	Osi2GlpkHeavyShim.cpp Osi2GlpkHeavyShim.hpp

libOsi2GlpkHeavyShim_la_LDFLAGS = $(LT_LDFLAGS) -module
libOsi2GlpkHeavyShim_la_LIBADD = $(OSI2GLPKHEAVYSHIM_LIBS)
endif


# Here list all include flags.
AM_CPPFLAGS = $(COINUTILS_CFLAGS) $(CLP_CFLAGS) \
	$(OSI2CLPSHIM_CFLAGS) $(OSI2GLPKHEAVYSHIM_CFLAGS)

# This line is necessary to allow VPATH compilation.
# This "cygpath" stuff is necessary to compile with native compilers on Windows.
DEFAULT_INCLUDES = -I. -I`$(CYGPATH_W) $(srcdir)` \
	-I`$(CYGPATH_W) $(srcdir)/../Osi2` \
	-I`$(CYGPATH_W) ../Osi2`


########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in 'includedir/coin'

includecoindir = $(includedir)/coin
#	Osi2Osi1API_Clp.hpp
includecoin_HEADERS = \
	Osi2ClpShim.hpp Osi2ProbMgmtAPI_Clp.hpp \
	Osi2ClpHeavyShim.hpp Osi2ProbMgmtAPI_ClpHeavy.hpp \
	Osi2Osi1API_ClpHeavy.hpp
if COIN_HAS_OSIGLPK
includecoin_HEADERS += Osi2GlpkHeavyShim.hpp Osi2Osi1API_GlpkHeavy.hpp
endif

