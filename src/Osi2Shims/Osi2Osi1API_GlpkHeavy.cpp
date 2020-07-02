/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2Osi1API_GlpkHeavy.cpp
    \brief Method definitions for Osi1API_GlpkHeavy

  Method definitions for Osi1API_GlpkHeavy, an implementation of the Osi1 API
  using the `heavy' glpk shim that's linked with glpk libraries and allows
  direct access to glpk objects.
*/

#include <iostream>

#include "Osi2GlpkHeavyShim.hpp"

#include "Osi2Config.h"
#include "Osi2nullptr.hpp"

#include "Osi2API.hpp"
#include "Osi2Osi1API.hpp"
#include "Osi2Osi1API_GlpkHeavy.hpp"

namespace Osi2 {

/*
  Constructor. There's nothing to do here; all the work happens in the
  constructors for the parent classes. The declarations here are just for
  convenient debugging.
*/
Osi1API_GlpkHeavy::Osi1API_GlpkHeavy ()
{
  /*
  std::cout << "Osi1API_Glpk object constructor." << std::endl ;
  */
}

/*
  Copy constructor
*/
Osi1API_GlpkHeavy::Osi1API_GlpkHeavy (const Osi1API_GlpkHeavy &rhs)
    : Osi1API(rhs),
      OsiGlpkSolverInterface(rhs)
{
  /*
  std::cout << "Osi1API_Glpk object copy constructor." << std::endl ;
  */
}

/*
  Destructor
*/
Osi1API_GlpkHeavy::~Osi1API_GlpkHeavy ()
{
  /*
  std::cout << "Osi1API_Glpk object destructor." << std::endl ;
  */
}


}    // end Osi2 namespace

