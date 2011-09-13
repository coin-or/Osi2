/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/

#ifndef Osi2ProbMgmtAPI_H
#define Osi2ProbMgmtAPI_H


/*! \brief Proof of concept API.

*/

namespace Osi2 {

class ProbMgmtAPI {

public:

 /// Virtual destructor
 virtual ~ProbMgmtAPI() {}

 /// Read an mps file from the given filename
 virtual
 int readMps(const char *filename, bool keepNames = false,
	     bool ignoreErrors = false) = 0 ;

} ;

}  // end namespace Osi2

#endif

