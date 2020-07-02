# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id: Makefile.am 1717 2011-05-15 18:52:52Z stefan $

# Author:  Andreas Waechter           IBM    2006-04-13

AUTOMAKE_OPTIONS = foreign

include BuildTools/Makemain.inc

########################################################################
#            Subdirectories and installation of .pc files              #
########################################################################

pkgconfiglibdir = $(libdir)/pkgconfig

pkgconfiglib_DATA = osi2.pc \
		    osi2plugin.pc \
		    osi2-unittests.pc \
		    osi2clpshim.pc \
		    osi2clpheavyshim.pc

SUBDIRS = src/Osi2Plugin \
	  src/Osi2 \
	  src/Osi2Shims

# We don't want to compile the test subdirectory, unless the test target is
# specified.  But we need to list it as subdirectory to make sure that it is
# included in the tarball. Similarly for cxxtest.

if ALWAYS_FALSE
  SUBDIRS += test cxxtest
endif

########################################################################
#             Additional files to be included in tarball               #
########################################################################

# Here we need include all files that are not mentioned in other Makefiles

EXTRA_DIST =

########################################################################
#                           Extra Targets                              #
########################################################################

test: all
	cd test; $(MAKE) test

if COIN_HAS_CXXTEST
cxxtest: all
	cd cxxtest ; $(MAKE) test
else
cxxtest: all
	@echo "CxxTest does not appear to be installed!"
endif

unitTest: test

clean-local: clean-doxygen-docs
	if test -r test/Makefile; then cd test; $(MAKE) clean; fi
	if test -r cxxtest/Makefile; then cd cxxtest; $(MAKE) clean; fi

distclean-local:
	if test -r test/Makefile; then cd test; $(MAKE) distclean; fi
	if test -r cxxtest/Makefile; then cd cxxtest; $(MAKE) distclean; fi

install-exec-local: install-doc

uninstall-local: uninstall-doc uninstall-doxygen-docs

.PHONY: test unitTest cxxtest

########################################################################
#                         Maintainer Stuff                             #
########################################################################

CLEANFILES =

# Files that are generated and should be cleaned with make distclean

DISTCLEANFILES =

