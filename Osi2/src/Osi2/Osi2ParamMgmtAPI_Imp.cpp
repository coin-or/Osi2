/*
  Copyright 2019 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$

  This file contains the implementation of the methods for the Osi2 Parameter
  Managememt API.
*/

// #include "CoinHelperFunctions.hpp"

#include "Osi2Config.h"

#include "Osi2API.hpp"
#include "Osi2ParamMgmtAPI.hpp"
#include "Osi2ParamMgmtAPI_Imp.hpp"

#include "Osi2nullptr.hpp"
#include "Osi2PluginManager.hpp"

namespace {

/*
  The `registration' instance. The sole purpose of this instance of
  ParamMgmtAPI is to register the initFunc with the PluginManager.
*/

Osi2::ParamMgmtAPI_Imp regInstance("ParamMgmt") ;

/*
  Plugin cleanup function.
*/
extern "C"
int32_t cleanupPlugin (const Osi2::PlatformServices *services)
{
  std::cout << "Executing Osi2ParamMgmt_Imp::cleanupPlugin." << std::endl ;
  return (0) ;
}

/*
  Plugin initialisation function. 
*/

extern "C"
Osi2::ExitFunc initPlugin (Osi2::PlatformServices *services)
{
  std::cout << "Executing Osi2ParamMgmt_Imp::initPlugin" << std::endl ;

  return (cleanupPlugin) ;
}
}  // end file-local namespace


namespace Osi2 {

/*
  Boilerplate: Constructors, destructors, & such like
*/

/*
  Default constructor
*/
ParamMgmtAPI_Imp::ParamMgmtAPI_Imp ()
  : pluginMgr_(0),
    logLvl_(7)
{
  msgHandler_ = new CoinMessageHandler() ;
  msgs_ = ParamMgmtAPIMessages() ;
  msgHandler_->setLogLevel(logLvl_) ;
  msgHandler_->message(PMMGAPI_INIT, msgs_)
      << "default" << CoinMessageEol ;
}

/*
  Registration constructor

  The sole purpose of this constructor is to register the ParamMgmt API
  with the plugin manager during program startup. The parameter exists to
  allow the compiler to choose the proper overload. Use this constructor
  to construct a static, file local instance that has absolutely no other use.
*/
ParamMgmtAPI_Imp::ParamMgmtAPI_Imp (std::string name)
  : pluginMgr_(0),
    dfltHandler_(true),
    msgHandler_(0),
    msgs_(0),
    logLvl_(0)
{
#ifndef NDEBUG
/*
  Useful for debugging.
*/
  msgHandler_ = new CoinMessageHandler() ;
  msgs_ = ParamMgmtAPIMessages() ;
  msgHandler_->setLogLevel(logLvl_) ;
  msgHandler_->message(PMMGAPI_INIT, msgs_)
      << "registration" << CoinMessageEol ;
#endif

  PluginManager &pm = PluginManager::getInstance() ;

  pm.addPreloadLib(name,initPlugin) ;
}

/*
  Copy constructor
*/
ParamMgmtAPI_Imp::ParamMgmtAPI_Imp (const ParamMgmtAPI_Imp &rhs)
    : pluginMgr_(rhs.pluginMgr_),
      dfltHandler_(rhs.dfltHandler_),
      logLvl_(rhs.logLvl_)
{
/*
  If this is our handler, make an independent copy. If it's the client's
  handler, we can't make an independent copy because the client won't know
  about it and won't delete it.
*/
  if (dfltHandler_) {
    msgHandler_ = new CoinMessageHandler(*rhs.msgHandler_) ;
  } else {
    msgHandler_ = rhs.msgHandler_ ;
  }
  msgs_ = rhs.msgs_ ;
  msgHandler_->setLogLevel(logLvl_) ;
  msgHandler_->message(PMMGAPI_INIT, msgs_)
    << "copy" << CoinMessageEol ;
}

/*
  Assignment
*/
ParamMgmtAPI_Imp &ParamMgmtAPI_Imp::operator= (const ParamMgmtAPI_Imp &rhs)
{
/*
  Self-assignment requires no work.
*/
  if (this == &rhs) return (*this) ;
/*
  Otherwise, get to it.
*/
  pluginMgr_ = rhs.pluginMgr_ ;
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
*/
ParamMgmtAPI_Imp::~ParamMgmtAPI_Imp ()
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
  Factory constructor
*/
ParamMgmtAPI *ParamMgmtAPI_Imp::create ()
{
  ParamMgmtAPI *api = new ParamMgmtAPI_Imp() ;
  return (api) ;
}

/*
  Clone (factory copy)
*/
ParamMgmtAPI *ParamMgmtAPI_Imp::clone ()
{
  ParamMgmtAPI *api = new ParamMgmtAPI_Imp(*this) ;
  return (api) ;
}



} // end namespace Osi2

