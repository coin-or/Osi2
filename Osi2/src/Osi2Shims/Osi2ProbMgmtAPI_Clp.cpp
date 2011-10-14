
/*
  Method definitions for ProbMgmtAPI_Clp, an implementation of the problem
  management API using Clp as the underlying solver.

  $Id$
*/


#include "Osi2ClpShim.hpp"
#include "Clp_C_Interface.h"
#include "ClpConfig.h"

#include "Osi2API.hpp"
#include "Osi2ProbMgmtAPI.hpp"
#include "Osi2ProbMgmtAPI_Clp.hpp"

namespace Osi2 {


/*
  Capture a pointer to the underlying ClpSimplex object.
*/
ProbMgmtAPI_Clp::ProbMgmtAPI_Clp (DynamicLibrary *libClp,
                                  Clp_Simplex *clpSimplex)
    : libClp_(libClp),
      clpSimplex_(clpSimplex)
{

}

ProbMgmtAPI_Clp::~ProbMgmtAPI_Clp ()
{
    /* Nothing to do yet. */
    std::cout
            << "ProbMgmtAPI_Clp object destroyed." << std::endl ;
}

/*
  And our non-trivial test method.
*/
int ProbMgmtAPI_Clp::readMps (const char *filename, bool keepNames,
                              bool ignoreErrors)
{
    typedef  int (*ClpReadMpsFunc)(Clp_Simplex*, const char*, int, int) ;
    std::string errStr ;
    ClpReadMpsFunc readMps =
        reinterpret_cast<ClpReadMpsFunc>
        (libClp_->getSymbol("Clp_readMps", errStr)) ;
    if (readMps == 0) {
        std::cout << "Apparent failure to find Clp_readMps." << std::endl ;
        std::cout << errStr << std::endl ;
        return (-1) ;
    }

    int retval = readMps(clpSimplex_, filename, keepNames, ignoreErrors) ;

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

}
