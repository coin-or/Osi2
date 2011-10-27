
/*
  Method definitions for Osi1API_Clp, an implementation of the problem
  management API using Clp as the underlying solver.

  $Id$
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

Osi1API_ClpHeavy::ApplyCutsReturnCode::~ApplyCutsReturnCode ()
{
  std::cout
    << "Osi1API_ClpHeavy::ApplyCutsReturnCode destructor." << std::endl ;
}

Osi1API_ClpHeavy::ApplyCutsReturnCode::ApplyCutsReturnCode
  (const OsiSolverInterface::ApplyCutsReturnCode &acrc)
  : Osi1API::ApplyCutsReturnCode(acrc.getNumInconsistent(),
  				 acrc.getNumInconsistentWrtIntegerModel(),
				 acrc.getNumInfeasible(),
				 acrc.getNumIneffective(),
				 acrc.getNumApplied())
  {}

/*
  Constructor. Gotta have one somewhere, I guess.
*/
Osi1API_ClpHeavy::Osi1API_ClpHeavy ()
{ 
  std::cout << "Osi1API_Clp object constructor." << std::endl ;
}

/*
  Copy constructor
*/
Osi1API_ClpHeavy::Osi1API_ClpHeavy (const Osi1API_ClpHeavy &rhs)
    : Osi1API(rhs),
      OsiClpSolverInterface(rhs)
  {
    std::cout << "Osi1API_Clp object copy constructor." << std::endl ;
  }

Osi1API_ClpHeavy::~Osi1API_ClpHeavy ()
{
  std::cout << "Osi1API_Clp object destructor." << std::endl ;
}


}

