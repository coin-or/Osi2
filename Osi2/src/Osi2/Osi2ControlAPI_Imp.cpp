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
ControlAPI_Imp::ControlAPI_Imp ()
  : logLvl_(7)
{
  msgHandler_ = new CoinMessageHandler() ;
  msgs_ = CtrlAPIMessages() ;
  msgHandler_->setLogLevel(logLvl_) ;
  msgHandler_->message(CTRLAPI_INIT,msgs_) << "default" << CoinMessageEol ;
}

/*
  Copy constructor

  Nothing to do so far.
*/
ControlAPI_Imp::ControlAPI_Imp (const ControlAPI_Imp &rhs)
  : dfltHandler_(rhs.dfltHandler_),
    logLvl_(rhs.logLvl_)
{
  if (dfltHandler_) {
    msgHandler_ = new CoinMessageHandler(*rhs.msgHandler_) ;
  } else {
    msgHandler_ = rhs.msgHandler_ ;
  }
  msgs_ = rhs.msgs_ ;
  msgHandler_->setLogLevel(logLvl_) ;
  msgHandler_->message(CTRLAPI_INIT,msgs_) << "copy" << CoinMessageEol ;
}

/*
  Assignment

  Nothing to do so far.
*/
ControlAPI_Imp &ControlAPI_Imp::operator= (const ControlAPI_Imp &rhs)
{
/*
  Self-assignment requires no work.
*/
  if (this == &rhs) return (*this) ;
/*
  If it's our handler, we need to delete the old and replace with the new.
  If it's the user's handler, it's the user's problem. We just assign the
  pointer.
*/
  if (dfltHandler_) {
    delete msgHandler_ ;
    msgHandler_ = nullptr ;
  }
  dfltHandler_ = rhs.dfltHandler_ ;
  if (dfltHandler_) {
    msgHandler_ = new CoinMessageHandler(*rhs.msgHandler_) ;
  } else {
    msgHandler_ = rhs.msgHandler_ ;
  }
  msgs_ = rhs.msgs_ ;
  msgHandler_->setLogLevel(logLvl_) ;

  return (*this) ;
}


/*
  Destructor

  Nothing to do so far.
*/
ControlAPI_Imp::~ControlAPI_Imp ()
{
/*
  If this is our handler, delete it. Otherwise it's the client's
  responsibility.
*/
  if (dfltHandler_) {
    delete msgHandler_ ;
    msgHandler_ = nullptr ;
  }
}

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

  I need to rethink my approach here. The Osi2 client wants access to an API.
  They really don't care about the `load' part. This should be a lazy
  operation, where Osi2 asks the plugin manager if something that supports
  this API is already loaded, we just return an object of the proper sort.
  If not, we ask for a load.

  But how clean can I make that? Some object, somewhere, must know the
  relation between the requested API and a solver library. Or how to find a
  default solver.

  Or maybe I want a blended interface. With the control API, the client can
  ask to load a particular plugin library, getting back a simply true/false.
  The business of asking for objects that implement particular APIs then
  becomes a different set of methods (createAPI, destroyAPI).
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
    int retval = pluginMgr_->loadOneLib("libOsi2ClpShim.so.0") ;
    if (retval < 0) {
    /*
      Ok, who should own the Osi2 message handler? The base API class?
      The ControlAPI class? Should we borrow it from the plugin manager?
    */
    }
  /*
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
  */
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

/*
  Just what does it mean to unload an API? Unloading a library has meaning,
  but for an individual API it's questionable.
*/
int ControlAPI_Imp::unload (API *api)
{
  return (-1) ;
}


/*
  Utility methods
*/


/*
  If we don't already have a pointer to the plugin manager, get it now.
*/
PluginManager *ControlAPI_Imp::findPluginMgr()
{
  if (pluginMgr_ == nullptr)
    pluginMgr_ = &PluginManager::getInstance() ;

  return (pluginMgr_) ;
}

} // end namespace Osi2
