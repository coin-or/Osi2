/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ProbMgmtAPI_Clp.hpp
    \brief Declarations for clp implementation of Osi2::ProbMgmtAPI
*/
#ifndef Osi2ProbMgmtAPI_Clp_HPP
#define Osi2ProbMgmtAPI_Clp_HPP

#include "Osi2DynamicLibrary.hpp"

#include "Osi2API.hpp"
#include "Osi2ProbMgmtAPI.hpp"

/*! \brief Proof of concept API.

  Capable of loading and solving a problem. Period.
*/

namespace Osi2 {

class ProbMgmtAPI_Clp : public ProbMgmtAPI {

public:
    /// Constructor with ClpSimplex object
    ProbMgmtAPI_Clp(DynamicLibrary *libClp, Clp_Simplex *clpSimplex) ;

    /// Destructor
    virtual ~ProbMgmtAPI_Clp() ;

    /// Read an mps file from the given filename
    int readMps(const char *filename, bool keepNames = false,
                bool ignoreErrors = false) ;

    /*! \brief Solve an lp

      See ClpModel::status() for the meaning of the return value.
    */
    int initialSolve() ;

private:
  /*! \name Dynamic object management information */
  //@{
    /// Dynamic library handle
    DynamicLibrary *libClp_ ;
    /// Clp object
    Clp_Simplex *clpSimplex_ ;
  //@}

  /*! \name Lazy symbol load

    Methods in the Clp C Interface are looked up as needed and cached. In
    general, there's a typedef and field for each method.
  */
  //@{
    typedef  int (*ClpReadMpsFunc)(Clp_Simplex*, const char*, int, int) ;
    ClpReadMpsFunc readMps_ ;
    typedef  int (*ClpInitialSolveFunc)(Clp_Simplex*) ;
    ClpInitialSolveFunc initialSolve_ ;
  //@}

} ;

}  // end namespace Osi2

#endif

