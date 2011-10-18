/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
#ifndef Osi2ProbMgmtAPI_ClpHeavy_HPP
#define Osi2ProbMgmtAPI_ClpHeavy_HPP

#include "Osi2DynamicLibrary.hpp"

#include "Osi2API.hpp"
#include "Osi2ProbMgmtAPI.hpp"

/*! \brief Proof of concept API.

*/

namespace Osi2 {

class ProbMgmtAPI_ClpHeavy : public ProbMgmtAPI {

public:
    /// Constructor with ClpSimplex object
    ProbMgmtAPI_ClpHeavy(ClpSimplex *clpSimplex) ;

    /// Destructor
    ~ProbMgmtAPI_ClpHeavy() ;

    /// Read an mps file from the given filename
    int readMps(const char *filename, bool keepNames = false,
                bool ignoreErrors = false) ;

    /*! \ Solve an lp

      See ClpModel::status() for the meaning of the return value.
    */
    int initialSolve() ;

private:
  /*! \name Dynamic object management information */
  //@{
    /// Clp object
    ClpSimplex *clpSimplex_ ;
  //@}

} ;

}  // end namespace Osi2

#endif

