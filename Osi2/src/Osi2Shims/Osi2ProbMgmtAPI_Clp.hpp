/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/

#ifndef Osi2ProbMgmtAPI_Clp_HPP
#define Osi2ProbMgmtAPI_Clp_HPP

#include "Osi2DynamicLibrary.hpp"
#include "Osi2ProbMgmtAPI.hpp"
#include "ClpSimplex.hpp"

/*! \brief Proof of concept API.

*/

namespace Osi2 {

class ProbMgmtAPI_Clp : public ProbMgmtAPI {

public:
  /// Constructor with ClpSimplex object
  ProbMgmtAPI_Clp(DynamicLibrary *libClp,Clp_Simplex *clpSimplex) ;

  /// Destructor
  ~ProbMgmtAPI_Clp() ;

  /// Read an mps file from the given filename
  int readMps(const char *filename, bool keepNames = false,
	     bool ignoreErrors = false) ;

private:
  DynamicLibrary *libClp_ ;
  Clp_Simplex *clpSimplex_ ;

} ;

}  // end namespace Osi2

#endif

