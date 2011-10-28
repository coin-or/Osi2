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
  Destructor
*/
Osi1API_ClpHeavy::ApplyCutsReturnCode::~ApplyCutsReturnCode ()
{
  /*
  std::cout
    << "Osi1API_ClpHeavy::ApplyCutsReturnCode destructor." << std::endl ;
  */
}

/*
  Additional constructor that knows about
  OsiSolverInterface::ApplyCutsReturnCode and can break it out into individual
  values to feed to the Osi1API::ApplyCutsReturnCode constructor.
*/
Osi1API_ClpHeavy::ApplyCutsReturnCode::ApplyCutsReturnCode
  (const OsiSolverInterface::ApplyCutsReturnCode &acrc)
  : Osi1API::ApplyCutsReturnCode(acrc.getNumInconsistent(),
  				 acrc.getNumInconsistentWrtIntegerModel(),
				 acrc.getNumInfeasible(),
				 acrc.getNumIneffective(),
				 acrc.getNumApplied())
{
  /*
  std::cout
    << "Osi1API_ClpHeavy::ApplyCutsReturnCode constructor from "
    << "OsiSolverInterface::ApplyCutsReturnCode object." << std::endl ;
  */
}

/*
  Constructor. There's nothing to do here; all the work happens in the
  constructors for the parent classes.
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

