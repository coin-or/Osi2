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
#include "Osi2ClpSimplexAPI_ClpLite.hpp"
#include "Osi2ClpSolveParamsAPI_ClpLite.hpp"

namespace Osi2 {

/// Default constructor
ClpLite_Wrap::ClpLite_Wrap ()
  : clp_(nullptr),
    clpSolve_(nullptr)
{ }
/// Destructor
ClpLite_Wrap::~ClpLite_Wrap ()
{ 
  delete clp_  ;
  delete clpSolve_ ;
}

void ClpLite_Wrap::addClpSimplex (DynamicLibrary *libClp)
{
/*
  Install a ClpSimplexAPI object and a ClpSolveParams object and add them to
  the set of supported APIs.
*/
  clp_ = new ClpSimplexAPI_ClpLite(libClp) ;
  apiMgr_.addAPIID(ClpSimplexAPI::getAPIIDString(),clp_) ;
  clpSolve_ = new ClpSolveParamsAPI_ClpLite(libClp) ;
  apiMgr_.addAPIID(ClpSolveParamsAPI::getAPIIDString(),clpSolve_) ;
}

}
