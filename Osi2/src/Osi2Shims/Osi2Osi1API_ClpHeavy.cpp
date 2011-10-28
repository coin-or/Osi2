/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2Osi1API_ClpHeavy.cpp
    \brief Method definitions for Osi1API_ClpHeavy

  Method definitions for Osi1API_ClpHeavy, an implementation of the Osi1 API
  using the `heavy' clp shim that's linked with clp libraries and allows
  direct access to clp objects.
*/

#include <iostream>

#include "ClpConfig.h"
#include "Osi2ClpShim.hpp"
#include "ClpSimplex.hpp"

#include "Osi2Config.h"
#include "Osi2nullptr.hpp"

#include "Osi2API.hpp"
#include "Osi2Osi1API.hpp"
#include "Osi2Osi1API_ClpHeavy.hpp"

namespace Osi2 {

/*
  Constructor. There's nothing to do here; all the work happens in the
  constructors for the parent classes. The declarations here are just for
  convenient debugging.
*/
Osi1API_ClpHeavy::Osi1API_ClpHeavy ()
{
  /*
  std::cout << "Osi1API_Clp object constructor." << std::endl ;
  */
}

/*
  Copy constructor
*/
Osi1API_ClpHeavy::Osi1API_ClpHeavy (const Osi1API_ClpHeavy &rhs)
    : Osi1API(rhs),
      OsiClpSolverInterface(rhs)
{
  /*
  std::cout << "Osi1API_Clp object copy constructor." << std::endl ;
  */
}

/*
  Destructor
*/
Osi1API_ClpHeavy::~Osi1API_ClpHeavy ()
{
  /*
  std::cout << "Osi1API_Clp object destructor." << std::endl ;
  */
}


}    // end Osi2 namespace

