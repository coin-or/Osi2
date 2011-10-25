
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


/*
  Constructor. Gotta have one somewhere, I guess.
*/
Osi1API_ClpHeavy::Osi1API_ClpHeavy ()
{ 
  std::cout << "Osi1API_Clp object constructed." << std::endl ;
}

Osi1API_ClpHeavy::~Osi1API_ClpHeavy ()
{
  std::cout << "Osi1API_Clp object destroyed." << std::endl ;
}


}

