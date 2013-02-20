# Copyright (C) 2006 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id: configure.ac 1752 2011-06-19 14:49:45Z stefan $

# Author:  Andreas Waechter            IBM    2006-04-13

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_PREREQ(2.59)

AC_INIT([Osi2],[trunk],[osi2@list.coin-or.org])

AC_COPYRIGHT([
Copyright 2006 International Business Machines and others.
All Rights Reserved.
This file is part of the open source package Coin which is distributed
under the Eclipse Public License.])

# List one file in the package so that the configure script can test
# whether the package is actually there
AC_CONFIG_SRCDIR(src/Osi2Plugin/Osi2Plugin.hpp)

# Where should everything be installed by default?  Here, we want it
# to be installed directly in 'bin', 'lib', 'include' subdirectories
# of the directory where configure is run.  The default would be
# /usr/local.
AC_PREFIX_DEFAULT([`pwd`])

#############################################################################
#                         Standard build tool stuff                         #
#############################################################################

# Get the system type
AC_CANONICAL_BUILD

# A bit of initial setup
AC_COIN_PROJECTDIR_INIT(Osi2)

# Check if user wants to produce debugging code
AC_COIN_DEBUG_COMPILE(Osi2)

# Get the name of the C++ compiler and appropriate compiler options
AC_COIN_PROG_CXX

# Osi2Plugin uses types defined in cstdint, and these require a compiler flag
# for acceptance. We also need to check for cstdint vs. stdint.h

AC_COIN_CHECK_CXX_CHEADER(stdint)

# See if we have native support for nullptr.

AC_LANG_PUSH(C++)
AC_MSG_CHECKING([for nullptr support])
AC_COMPILE_IFELSE(
 [AC_LANG_PROGRAM([],[[double *ptr ; ptr = nullptr ;]])],
 [AC_DEFINE(COIN_HAS_NULLPTR,[1],
 	    [Define to 1 if the C++ compiler supports nullptr])
  ac_cv_type_nullptr=yes
  AC_MSG_RESULT(yes)],
 [ ac_cv_type_nullptr=no
   AC_MSG_RESULT(no)])
AC_LANG_POP()

# Initialize automake and libtool
AC_COIN_INIT_AUTO_TOOLS

#############################################################################
#                              COIN-OR components                           #
#############################################################################

AC_COIN_CHECK_PACKAGE(CoinUtils, [coinutils], [Osi2 Osi2Plugin])
if test $coin_has_coinutils != yes ; then
  AC_MSG_ERROR([Required package CoinUtils not available.])
fi

# One can build ClpShim without libClp, but ClpHeavyShim must be linked with
# libOsiClp and dependencies. But see below for separate additions to CFLAGS.

AC_COIN_CHECK_PACKAGE(OsiClp, [osi-clp], [Osi2ClpHeavyShim])
if test $coin_has_osiclp != yes ; then
  AC_MSG_ERROR([Required package OsiClp not available.])
fi

# Similarly, GlpkHeavyShim needs libOsiGlpk and dependencies.
AC_COIN_CHECK_PACKAGE(OsiGlpk, [osi-glpk], [Osi2GlpkHeavyShim])

# Can we find CxxTest? We'll need it for the unit test. If there's a ThirdParty
# project, use it. Otherwise, look for a system installation.
AC_COIN_CHECK_PACKAGE(CxxTest, [coincxxtest], [CxxUnitTestDeps])
if test $coin_has_cxxtest = yes ; then
  AC_SUBST(CXXTESTGEN)
  if test -x "${prefix}/bin/cxxtestgen" ; then
    CXXTESTGEN="\$(prefix)/bin/cxxtestgen"
  elif test -x "${srcdir}/../ThirdParty/CxxTest/cxxtest/bin/cxxtestgen" ; then
    CXXTESTGEN="\$(top_srcdir)/../ThirdParty/CxxTest/cxxtest/bin/cxxtestgen"
  fi
else
  AC_PATH_PROG(CXXTESTGEN,cxxtestgen)
  if test -n "$CXXTESTGEN" ; then
    coin_has_cxxtest=yes
  fi
fi
# AC_MSG_NOTICE([Using $CXXTESTGEN])

# It sure would be handy to use the pkgconfig machinery to build up
# dependencies. Problem: On the first configure pass, none of the .pc files
# exist.  So, on the first pass, fake it by manually building up the necessary
# variables.  (The alternative is to build them up in the makefiles. This way
# the makefiles look like normal usage.)  Because the light shim (Osi2ClpShim)
# has no prior dependencies, the variables OSI2CLPSHIM_XXX don't exist yet
# (because there's been no reference to Osi2ClpShim in a package check)
# and must be created. Similarly UNITTESTDEPS_XXX. We need to be careful
# to delay expansion of variables that won't have a value until the final
# Makefile is generated.

# The reason for Osi2PluginInt is that using the name Osi2Plugin will result
# in OSI2PLUGIN_PCREQUIRES = osi2plugin, which we do not want. (This can't be
# fixed in coin.m4 because it's a feature when constructing a .pc file
# for a synthetic package.) But we would like to have pkg-config collect the
# dependencies of osi2plugin and augment the dependency info for the listed
# packages. Variables OSI2PLUGININT_XXX are never used.

# UNITTESTDEPS serves a different purpose. It exists so that we have
# convenient variables UNITTESTDEPS_LIBS and UNITTESTDEPS_CFLAGS to use in
# test/Makefile.am.

# It's necessary to add osi2plugin to OSI2CLPSHIM_PCREQUIRES and
# OSI2_PCREQUIRES so that we end up with properly-formed Requires specs in the
# .pc files.

if test -f osi2plugin.pc ; then
  AC_MSG_NOTICE([Found .pc files; invoking pkg-config.])
  AC_COIN_CHECK_PACKAGE(Osi2PluginInt, [osi2plugin],
      [Osi2 Osi2ClpShim Osi2ClpHeavyShim Osi2GlpkHeavyShim])
  AC_COIN_CHECK_PACKAGE(UnitTestDeps, [osi osi2])
  AC_COIN_CHECK_PACKAGE(CxxUnitTestDeps, [osi osi2 coincxxtest])
else
  AC_MSG_NOTICE([No .pc files; faking it.])
  # Create variables
  AM_CONDITIONAL(COIN_HAS_OSI2PLUGININT,true)
  AM_CONDITIONAL(COIN_HAS_UNITTESTDEPS,true)
  AM_CONDITIONAL(COIN_HAS_CXXUNITTESTDEPS,
  		 [test $coin_has_cxxtest != notGiven &&
		  test $coin_has_cxxtest != skipping])
  AC_SUBST(OSI2CLPSHIM_LIBS)
  AC_SUBST(OSI2CLPSHIM_CFLAGS)
  AC_SUBST(OSI2CLPSHIM_PCREQUIRES, [osi2plugin])
  OSI2CLPSHIM_LIBS="\$(top_builddir)/src/Osi2Plugin/libOsi2Plugin.la"
  OSI2CLPSHIM_CFLAGS="-I\$(top_srcdir)/src/Osi2Plugin"
  OSI2CLPHEAVYSHIM_LIBS="\$(top_builddir)/src/Osi2Plugin/libOsi2Plugin.la $OSI2CLPHEAVYSHIM_LIBS"
  OSI2CLPHEAVYSHIM_CFLAGS="-I\$(top_srcdir)/src/Osi2Plugin $OSI2CLPHEAVYSHIM_CFLAGS"
  OSI2CLPHEAVYSHIM_PCREQUIRES="osi2plugin $OSI2CLPHEAVYSHIM_PCREQUIRES"
  OSI2GLPKHEAVYSHIM_LIBS="\$(top_builddir)/src/Osi2Plugin/libOsi2Plugin.la $OSI2GLPKHEAVYSHIM_LIBS"
  OSI2GLPKHEAVYSHIM_CFLAGS="-I\$(top_srcdir)/src/Osi2Plugin $OSI2GLPKHEAVYSHIM_CFLAGS"
  OSI2GLPKHEAVYSHIM_PCREQUIRES="osi2plugin $OSI2GLPKHEAVYSHIM_PCREQUIRES"
  OSI2_CFLAGS="-I\$(top_srcdir)/src/Osi2Plugin $OSI2_CFLAGS"
  OSI2_LIBS="\$(top_builddir)/src/Osi2Plugin/libOsi2Plugin.la $OSI2_LIBS"
  OSI2_PCREQUIRES="osi2plugin $OSI2_PCREQUIRES"
  AC_SUBST(UNITTESTDEPS_LIBS)
  AC_SUBST(UNITTESTDEPS_CFLAGS)
  UNITTESTDEPS_LIBS="\$(top_builddir)/src/Osi2/libOsi2.la $OSI2_LIBS"
  UNITTESTDEPS_CFLAGS="-I\$(top_srcdir)/src/Osi2 $OSI2_CFLAGS"
  UNITTESTDEPS_CFLAGS="-I\$(top_srcdir)/../Osi/src/Osi $UNITTESTDEPS_CFLAGS"

# CxxUnitTest libs should be the same as regular UnitTest libs, but don't
# define in those terms because regular unit test may disappear. CFLAGS needs
# the additional include directories for the CxxTest source.
  CXXUNITTESTDEPS_LIBS="\$(top_builddir)/src/Osi2/libOsi2.la $OSI2_LIBS"
  CXXUNITTESTDEPS_CFLAGS="-I\$(top_srcdir)/src/Osi2 $OSI2_CFLAGS"
  CXXUNITTESTDEPS_CFLAGS="-I\$(top_srcdir)/../Osi/src/Osi $CXXUNITTESTDEPS_CFLAGS"
  CXXUNITTESTDEPS_CFLAGS="-I\$(top_srcdir)/../ThirdParty/CxxTest/cxxtest $CXXUNITTESTDEPS_CFLAGS"
fi

# So we can use OSI2PLUGIN in makefiles instead of OSI2PLUGININT. Not clear
# it's necessary at present.

AM_CONDITIONAL(COIN_HAS_OSI2PLUGIN,true)

# Augment OSI2CLPSHIM_CFLAGS (used when building both the light and heavy
# shims) so we can find all necessary include files. Do this explicitly to
# avoid dragging in libs we don't need. I should figure out how to
# use separate CFLAGS variables in Makefile.am. -- lh, 111021 --

OSI2CLPSHIM_CFLAGS="$OSI2CLPSHIM_CFLAGS -I\$(top_srcdir)/../Clp/src"
OSI2CLPSHIM_CFLAGS="$OSI2CLPSHIM_CFLAGS -I\$(top_srcdir)/../Clp/src/OsiClp"
OSI2CLPSHIM_CFLAGS="$OSI2CLPSHIM_CFLAGS -I\$(top_builddir)/../Clp/src"
OSI2CLPSHIM_CFLAGS="$OSI2CLPSHIM_CFLAGS -I\$(top_srcdir)/../Osi/src/Osi"
OSI2CLPSHIM_CFLAGS="$OSI2CLPSHIM_CFLAGS -I\$(top_builddir)/../Osi/src/Osi"
OSI2CLPSHIM_CFLAGS="$OSI2CLPSHIM_CFLAGS -I\$(top_srcdir)/src/Osi2"

# Avoid pulling Osi2Config.h and dependencies into the build for libOsi2Plugin
# just for COIN_HAS_NULLPTR. Eventually nullptr should move to CoinUtils.

if test "x$ac_cv_type_nullptr" = xyes ; then
  OSI2PLUGIN_CFLAGS="$OSI2PLUGIN_CFLAGS -DCOIN_HAS_NULLPTR"
fi

AC_COIN_CHECK_PACKAGE(Sample,    [coindatasample])
AC_COIN_CHECK_PACKAGE(Netlib,    [coindatanetlib])

#############################################################################
#                            Third party solvers                            #
#############################################################################

# Check which third party solvers are available. Cplex detection has been
# tested on a semi-regular basis, and Mosek detection most likely works. For
# the rest, it's anyone's guess.  -- lh, 080529 --
# fixed detection of Mosek and Xpress -- stefan, 091003 (linux32, gcc) --

AC_COIN_CHECK_USER_LIBRARY([Cplex], [CPX],[cplex.h],   [CPXgetstat])
AC_COIN_CHECK_USER_LIBRARY([Mosek], [MSK],[mosek.h],   [MSK_makeenv])
AC_COIN_CHECK_USER_LIBRARY([Xpress],[XPR],[xprs.h],    [XPRSinit])
AC_COIN_CHECK_USER_LIBRARY([Gurobi],[GRB],[gurobi_c.h],[GRBloadenv])

#############################################################################
#                            Examples solver                                #
#############################################################################

# choose a solver interface that can be used in the examples

if test x$coin_has_glpk = xyes ; then
  OSI_EXAMPLES_SOLVER_NAME=OsiGlpkSolverInterface
  OSI_EXAMPLES_SOLVER_CFLAGS="$GLPK_CFLAGS"
  OSI_EXAMPLES_SOLVER_LIBS="-lOsiGlpk $GLPK_LIBS_INSTALLED"
  OSI_EXAMPLES_SOLVER_PCNAME=osi-glpk
fi

# adjust linker flags for (i)cl compiler
if test x$coin_cxx_is_cl = xtrue || test x$coin_cc_is_cl = xtrue ;
then
  [OSI2_EXAMPLES_SOLVER_LIBS=`echo " $OSI2_EXAMPLES_SOLVER_LIBS" | sed -e 's/ -L\([^ ]*\)/ \/libpath:\`$(CYGPATH_W) \1\`/g' -e 's/ -l\([^ ]*\)/ lib\1.lib/g'`]
fi

AC_SUBST(OSI2_EXAMPLES_SOLVER_NAME)
AC_SUBST(OSI2_EXAMPLES_SOLVER_CFLAGS)
AC_SUBST(OSI2_EXAMPLES_SOLVER_LIBS)
AC_SUBST(OSI2_EXAMPLES_SOLVER_PCNAME)


#############################################################################
#                 Configuration options for individual OSIs                 #
#############################################################################

# If any of the tests performed in this section actually require the presence
# of the solver (file presence, link checks, etc.) be sure to guard the call.

# We assume that GLPK is not too old

AC_DEFINE(GLPK_HAS_INTOPT, [1],
	  [Define to 1 if GLPK has the advanced B&B solver lpx_intopt])

#############################################################################
#                  Check for doxygen                                        #
#############################################################################

AC_COIN_DOXYGEN(CoinUtils)

#############################################################################
#                  System header                                            #
#############################################################################

AC_COIN_CHECK_CXX_CHEADER(math)
AC_COIN_CHECK_CXX_CHEADER(float)
AC_COIN_CHECK_CXX_CHEADER(ieeefp)

##############################################################################
#                   Finishing up by writing all the output                   #
##############################################################################

# Here list all the files that configure should create (except for the
# configuration header file)
AC_CONFIG_FILES([Makefile
                 test/Makefile
                 cxxtest/Makefile
		 src/Osi2Plugin/Makefile
		 src/Osi2/Makefile
		 src/Osi2Shims/Makefile
                 osi2.pc osi2-uninstalled.pc
                 osi2plugin.pc osi2plugin-uninstalled.pc
                 osi2clpshim.pc osi2clpshim-uninstalled.pc
                 osi2clpheavyshim.pc osi2clpheavyshim-uninstalled.pc
                 osi2-unittests.pc osi2-unittests-uninstalled.pc])

AC_CONFIG_FILES([doxydoc/doxygen.conf])

# Here put the location and name of the configuration header file
AC_CONFIG_HEADER([src/Osi2/config.h src/Osi2/config_osi2.h])

# Finally, we let configure write all the output...
AC_COIN_FINALIZE
