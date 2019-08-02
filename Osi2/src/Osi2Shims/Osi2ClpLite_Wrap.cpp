/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ClpLite_Wrap.cpp
    \brief Method definitions for Osi2::ClpLite_Wrap.

  A experiment in implementing APIs by composition. This class is intended as
  a container for objects implementing specific APIs.
*/

#include "Osi2ClpLite_Wrap.hpp"

namespace Osi2 {

class ClpSimplexAPI ;
class ClpSimplexAPI_ClpLite ;

void ClpLite_Wrap::addClpSimplex (DynamicLibrary *libClp)
{
/*
  Install a ClpSimplexAPI object and add it to the set of supported APIs.
*/
  clp_ = new ClpSimplexAPI_ClpLite(libClp) ;
  apiMgr_.addAPIID(ClpSimplexAPI::getAPIIDString(),clp_) ;
}

}
