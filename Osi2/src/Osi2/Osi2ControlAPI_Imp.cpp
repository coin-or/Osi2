/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$

  This file contains the implementation of the methods for the Osi2 Control
  API.
*/

#include <iostream>

#include "Osi2API.hpp"
#include "Osi2ControlAPI.hpp"
#include "Osi2ControlAPI_Imp.hpp"

#include "Osi2PluginManager.hpp"
#include "Osi2DynamicLibrary.hpp"


namespace Osi2 {

/*
  Boilerplate: Constructors, destructors, & such like
*/

/*
  Default constructor

  Nothing to do so far.
*/
ControlAPI_Imp::ControlAPI_Imp () { }

/*
  Copy constructor

  Nothing to do so far.
*/
ControlAPI_Imp::ControlAPI_Imp (const ControlAPI_Imp &original) { }

/*
  Assignment

  Nothing to do so far.
*/
ControlAPI_Imp &ControlAPI_Imp::operator= (const ControlAPI_Imp &rhs)
{
  if (this != &rhs) {
    /* nothing to do yet */
  }
  return (*this) ;
}

/*
  Destructor

  Nothing to do so far.
*/
ControlAPI_Imp::~ControlAPI_Imp () { }

/*
  Virtual constructor
*/
ControlAPI *ControlAPI_Imp::create ()
{
  ControlAPI *api = new ControlAPI_Imp() ;
  return (api) ;
}

/*
  Clone
*/
ControlAPI *ControlAPI_Imp::clone ()
{
  ControlAPI *api = new ControlAPI_Imp(*this) ;
  return (api) ;
}

/*
  Load and unload
*/

API *ControlAPI_Imp::load (std::string api, std::string solver, int &rtncode)
{
  rtncode = -1 ;
  API *apiPtr = nullptr ;

/*
  Attempt to find the plugin manager.
*/
  if (findPluginMgr() == nullptr) {
    std::cout
      << "Osi2::ControlAPI::load: Apparent failure to find plugin manager."
      << std::endl ;
    rtncode = -2 ;
    return (apiPtr) ;
  }
/*
  Ask the plugin manager to load the solver.
*/
  std::string dfltDir = pluginMgr_->getDfltPluginDir() ;
  if (solver.compare("clp") == 0) {
    std::string clpShimPath = dfltDir+"/"+"libOsi2ClpShim.so.0" ;
    std::string errMsg ;
    DynamicLibrary *clpShim = DynamicLibrary::load(clpShimPath,errMsg) ;
    if (clpShim == 0) {
      std::cout
	<< "Apparent failure opening " << clpShimPath << "." << std::endl ;
      std::cout
	<< "Error is " << errMsg << "." << std::endl ;
      rtncode = -3 ;
      return (apiPtr) ;
    }
  }
/*
  Check that the solver implements the required API.
*/
  /*
    Sure. How? We could return a list of capabilities as part of the
    registration info for the solver.  Or we could define a new capabilities
    inquiry method.

    And the decision will be to define an inquiry method that the solver shim
    should provide. We can pass a list of capabilities in a string, separated
    by some appropriate delimiter. The solver shim can look at the set of
    capabilities, decide if it's capable of providing them simultaneously,
    and return a boolean result.
  */
    
  return (apiPtr) ;
}

int ControlAPI_Imp::unload (API *api)
{
  return (-1) ;
}


/*
  Utility methods
*/
PluginManager *ControlAPI_Imp::findPluginMgr()
{
  if (pluginMgr_ == nullptr)
    pluginMgr_ = &PluginManager::getInstance() ;

  return (pluginMgr_) ;
}

} // end namespace Osi2
