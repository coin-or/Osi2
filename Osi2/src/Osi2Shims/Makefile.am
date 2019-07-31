# Copyright (C) 2010 Lou Hafer
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id$

# Author:  Lou Hafer        SFU        2010-07-29

AUTOMAKE_OPTIONS = foreign

include $(top_srcdir)/BuildTools/Makemain.inc

# Names of the libraries compiled in this directory.

lib_LTLIBRARIES = libOsi2ClpShim.la

########################################################################
#                      libOsi2ClpShim, ClpHeavyShim                    #
########################################################################

# List all source files for this library, including headers. The LDFLAGS
# variables are consumed by libtool.

libOsi2ClpShim_la_SOURCES = \
	Osi2ProbMgmtAPI_Clp.cpp Osi2ProbMgmtAPI_Clp.hpp \
	Osi2ClpLite_Wrap.cpp Osi2ClpLite_Wrap.hpp \
	Osi2ClpSimplexAPI_ClpLite.cpp Osi2ClpSimplexAPI_ClpLite.hpp \
	Osi2ClpShim.cpp Osi2ClpShim.hpp

libOsi2ClpShim_la_LDFLAGS = $(LT_LDFLAGS) -module

libOsi2ClpShim_la_LIBADD = $(OSI2CLPSHIM_LFLAGS)

AM_CPPFLAGS = -DOSI2_BUILD \
	      -I$(srcdir)/../Osi2Plugin \
	      -I$(srcdir)/../Osi2 \
	      -I$(pkgincludedir)

#if COIN_HAS_OSICLP

lib_LTLIBRARIES += libOsi2ClpHeavyShim.la

libOsi2ClpHeavyShim_la_SOURCES = \
	Osi2ProbMgmtAPI_ClpHeavy.cpp Osi2ProbMgmtAPI_ClpHeavy.hpp \
	Osi2Osi1API_ClpHeavy.cpp Osi2Osi1API_ClpHeavy.hpp \
	Osi2ClpHeavyShim.cpp Osi2ClpHeavyShim.hpp

libOsi2ClpHeavyShim_la_LDFLAGS = $(LT_LDFLAGS) -module

libOsi2ClpHeavyShim_la_LIBADD = $(OSI2CLPHEAVYSHIM_LFLAGS)

AM_CPPFLAGS += $(OSI2CLPHEAVYSHIM_CFLAGS)

#endif



########################################################################
#                      libOsi2GlpkHeavyShim                            #
########################################################################

#if COIN_HAS_OSIGLPK

lib_LTLIBRARIES += libOsi2GlpkHeavyShim.la

libOsi2GlpkHeavyShim_la_SOURCES = \
	Osi2Osi1API_GlpkHeavy.cpp Osi2Osi1API_GlpkHeavy.hpp \
	Osi2GlpkHeavyShim.cpp Osi2GlpkHeavyShim.hpp

libOsi2GlpkHeavyShim_la_LDFLAGS = $(LT_LDFLAGS) -module

libOsi2GlpkHeavyShim_la_LIBADD = $(OSI2GLPKHEAVYSHIM_LFLAGS)

AM_CPPFLAGS += $(OSI2GLPKHEAVYSHIM_CFLAGS)

#endif

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in pkgincludedir

includecoindir = $(includedir)/coin-or

includecoin_HEADERS = Osi2ClpShim.hpp Osi2ProbMgmtAPI_Clp.hpp \
		      Osi2ClpLite_Wrap.hpp Osi2ClpSimplexAPI_ClpLite.hpp

#if COIN_HAS_OSICLP
includecoin_HEADERS += Osi2ClpHeavyShim.hpp Osi2ProbMgmtAPI_ClpHeavy.hpp \
		       Osi2Osi1API_ClpHeavy.hpp
#endif

if COIN_HAS_OSIGLPK
includecoin_HEADERS += Osi2GlpkHeavyShim.hpp Osi2Osi1API_GlpkHeavy.hpp
endif

