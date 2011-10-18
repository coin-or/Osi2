
/*
  Method definitions for ProbMgmtAPI_Clp, an implementation of the problem
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
#include "Osi2ProbMgmtAPI.hpp"
#include "Osi2ProbMgmtAPI_ClpHeavy.hpp"

namespace Osi2 {


/*
  Capture a pointer to the underlying ClpSimplex object.
*/
ProbMgmtAPI_ClpHeavy::ProbMgmtAPI_ClpHeavy (ClpSimplex *clpSimplex)
    : clpSimplex_(clpSimplex)
{
}

ProbMgmtAPI_ClpHeavy::~ProbMgmtAPI_ClpHeavy ()
{
  delete clpSimplex_ ;
  std::cout << "ProbMgmtAPI_Clp object destroyed." << std::endl ;
}

/*
  Read a problem file in mps format.
*/
int ProbMgmtAPI_ClpHeavy::readMps (const char *filename, bool keepNames,
                              bool ignoreErrors)
{
    int retval = clpSimplex_->readMps(filename, keepNames, ignoreErrors) ;

    if (retval) {
        std::cout
                << "Failure to read " << filename << ", error " << retval
                << "." << std::endl ;
    } else {
        std::cout
                << "Read " << filename << " without error." << std::endl ;
    }

    return (retval) ;
}

/*
  Solve a problem.
*/
int ProbMgmtAPI_ClpHeavy::initialSolve ()
{
    int retval = clpSimplex_->initialSolve() ;

    if (retval < 0) {
        std::cout
                << "Solve failed; error " << retval << "." << std::endl ;
    } else {
        std::cout
                << "Solved; return status " << retval << "." << std::endl ;
    }

    return (retval) ;
}

}
