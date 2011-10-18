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

AC_MSG_CHECKING([for nullptr support])
AC_LANG_PUSH(C++)
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

AC_COIN_CHECK_PACKAGE(CoinUtils, [coinutils],
    [Osi2Lib Osi2Plugin Osi2ClpHeavyShim])
if test $coin_has_coinutils != yes ; then
  AC_MSG_ERROR([Required package CoinUtils not available.])
fi

# One can build ClpShim without libClp, but ClpHeavyShim must be linked with
# libClp and dependencies.

AC_COIN_CHECK_PACKAGE(Clp, [clp], [Osi2ClpHeavyShim])
if test $coin_has_clp != yes ; then
  AC_MSG_ERROR([Required package Clp not available.])
fi

# The problem: On the first pass through configuration, none of the .pc
# files exist. But the shim needs osi2plugin.pc to find headers. Fake it by
# manually building up the necessary variables. This won't be a problem when
# (if) the clp shim moves over to the Clp project directory. It's just that
# all the .pc files are created at the end of this script and we need to
# get through the first call.

if test -f osi2plugin.pc ; then
  echo "Found osi2plugin.pc; invoking pkg-config."
  AC_COIN_CHECK_PACKAGE(Osi2Plugin, [osi2plugin],
      [Osi2ClpShim Osi2ClpHeavyShim])
  if test $coin_has_osi2plugin != yes ; then
    AC_MSG_ERROR([Required package Osi2Plugin not available.])
  fi
else
  echo "No osi2plugin.pc; faking it."
  AM_CONDITIONAL(COIN_HAS_OSI2PLUGIN,true)
  AC_SUBST(OSI2CLPSHIM_LIBS)
  AC_SUBST(OSI2CLPSHIM_CFLAGS)
  OSI2CLPSHIM_LIBS=../Osi2Plugin/libOsi2Plugin.la
  OSI2CLPSHIM_CFLAGS=-I$abs_source_dir/src/Osi2Plugin
  AC_SUBST(OSI2CLPHEAVYSHIM_LIBS)
  AC_SUBST(OSI2CLPHEAVYSHIM_CFLAGS)
  OSI2CLPHEAVYSHIM_LIBS=../Osi2Plugin/libOsi2Plugin.la
  OSI2CLPHEAVYSHIM_CFLAGS=-I$abs_source_dir/src/Osi2Plugin
fi
OSI2CLPSHIM_CFLAGS="$OSI2CLPSHIM_CFLAGS -I$abs_source_dir/src/Osi2"
OSI2CLPHEAVYSHIM_CFLAGS="$OSI2CLPHEAVYSHIM_CFLAGS -I$abs_source_dir/src/Osi2"

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
		 src/Osi2Plugin/Makefile
		 src/Osi2/Makefile
		 src/Osi2Shims/Makefile
                 osi2.pc osi2-uninstalled.pc
                 osi2plugin.pc osi2plugin-uninstalled.pc
                 osi2-unittests.pc osi2-unittests-uninstalled.pc])

AC_CONFIG_FILES([doxydoc/doxygen.conf])

# Here put the location and name of the configuration header file
AC_CONFIG_HEADER([src/Osi2/config.h src/Osi2/config_osi2.h])

# Finally, we let configure write all the output...
AC_COIN_FINALIZE
