/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$

  This file contains the implementation of the methods for the Osi2 Control
  API.
*/

#include "CoinHelperFunctions.hpp"

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
  knownLibMap_.clear() ;
  msgHandler_ = new CoinMessageHandler() ;
  msgs_ = CtrlAPIMessages() ;
  msgHandler_->setLogLevel(logLvl_) ;
  msgHandler_->message(CTRLAPI_INIT,msgs_) << "default" << CoinMessageEol ;
}

/*
  Copy constructor
*/
ControlAPI_Imp::ControlAPI_Imp (const ControlAPI_Imp &rhs)
  : dfltHandler_(rhs.dfltHandler_),
    logLvl_(rhs.logLvl_)
{
  knownLibMap_ = rhs.knownLibMap_ ;
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
  msgHandler_->message(CTRLAPI_INIT,msgs_) << "copy" << CoinMessageEol ;
}

/*
  Assignment
*/
ControlAPI_Imp &ControlAPI_Imp::operator= (const ControlAPI_Imp &rhs)
{
/*
  Self-assignment requires no work.
*/
  if (this == &rhs) return (*this) ;
/*
  Otherwise, get to it.
*/
  knownLibMap_ = rhs.knownLibMap_ ;
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
ControlAPI_Imp::~ControlAPI_Imp ()
{
  knownLibMap_.clear() ;
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

  This set of methods provides some sugar overtop of the PluginManager for
  library load/unload. The business of asking for objects that implement
  particular APIs is a different set of methods (createAPI, destroyAPI).

  The PluginManager load method expects a library name and a directory. The
  bottom ControlAPI method takes exactly those parameters and calls
  PluginManager::load. The next level up expects just the library name and
  uses the default plugin directory. The top level expects just a short
  name, xxx, and constructs the library name as libXxxShim.so.

  As a convenience (and to easily allow alternate plugin libraries), the
  ControlAPI keeps a map that associates shortName with the libName and
  dirName.

  Returns:
   -4: no plugin manager
   -3: initFunc failed
   -2: no initFunc
   -1: library failed to load
    0: library loaded and initialised without error
    1: library is already loaded

  With the exception of -4, all return codes should match loadOneLib.
*/

int ControlAPI_Imp::load (const std::string &shortName,
			  const std::string &libName,
			  const std::string *dirName)
{
  int retval = -1 ;
/*
  Already loaded?
*/
  LibMapType::iterator knownIter = knownLibMap_.find(shortName) ;
  if (knownIter != knownLibMap_.end()) return (1) ;
/*
  Not already loaded. Find the plugin manager.
*/
  if (findPluginMgr() == nullptr) {
    msgHandler_->message(CTRLAPI_NOPLUGMGR,msgs_) << CoinMessageEol ;
    retval = -4 ;
    return (retval) ;
  }
/*
  Construct a full path and ask the plugin manager to load the solver. If
  we're successful, enter it into the known libraries map.
*/
  std::string fullPath = libName ;
  if (dirName != nullptr && (*dirName) != "") {
    char dirSep = CoinFindDirSeparator() ;
    std::string fullPath = (*dirName)+dirSep+fullPath ;
  }
  retval = pluginMgr_->loadOneLib(fullPath) ;
  if (retval < 0) return (retval) ;
  if (retval == 1) {
    msgHandler_->message(CTRLAPI_UNREG,msgs_)
      << fullPath << shortName << CoinMessageEol ;
  }
  knownLibMap_[shortName] = fullPath ;
  msgHandler_->message(CTRLAPI_LOADOK,msgs_)
    << fullPath << shortName << CoinMessageEol ;
  
  return (retval) ;
}

/*
  Determine the default directory and call the base load method.

  Check first to see if we know a default. As a last resort, check the
  PluginManager. (Remember, there's only one PluginManager, whereas there can
  be multiple ControlAPI objects.)
*/
int ControlAPI_Imp::load (const std::string &shortName,
			  const std::string &libName)
{
  int retval = -1 ;
/*
  Try to find a default plugin directory. First our own, then consult the
  plugin manager. Both can come up empty.
*/
  std::string dirName = getDfltPluginDir() ;
  if (dirName == "") {
    if (findPluginMgr() == nullptr) {
      msgHandler_->message(CTRLAPI_NOPLUGMGR,msgs_) << CoinMessageEol ;
      retval = -4 ;
      return (retval) ;
    }
    dirName = pluginMgr_->getDfltPluginDir() ;
  }
  retval = load(shortName,libName,&dirName) ;

  return (retval) ;
}

/*
  Maximum syntactic sugar. Given the short name, construct a library name and
  call the next method in the hierarchy.
*/
int ControlAPI_Imp::load (const std::string &shortName)
{
  int retval = -1 ;

  std::string libName = "libOsi2" ;
  std::string::const_iterator firstChar = shortName.begin() ;
  const char ucChar = static_cast<char>(toupper(*firstChar)) ;
  libName += ucChar+shortName.substr(1,std::string::npos)+
  	     "Shim.so" ;
  retval = load(shortName,libName) ;
  return (retval) ;
}

/*
  Unload a library.

  Returns:
    -2: No plugin manager
    -1: exit function failed
     0: library unloaded successfully
     1: library is not loaded (PluginManager)
     2: library is not registered (ControlAPI)
*/
int ControlAPI_Imp::unload (const std::string &shortName)
{
  int retval = -1 ;
/*
  Look for the map entry in known libraries. Return if we don't find it.
*/
  LibMapType::iterator knownIter = knownLibMap_.find(shortName) ;
  if (knownIter == knownLibMap_.end()) {
    msgHandler_->message(CTRLAPI_UNREG,msgs_) ;
    msgHandler_->printing(false) << "" ;
    msgHandler_->printing(true) << shortName << CoinMessageEol ;
    retval = 2 ;
    return (retval) ;
  }
/*
  Make sure we can find the plugin manager.
*/
  if (findPluginMgr() == nullptr) {
    msgHandler_->message(CTRLAPI_NOPLUGMGR,msgs_) << CoinMessageEol ;
    retval = -2 ;
    return (retval) ;
  }
/*
  Separate the libName and directory, then call the plugin manager's unload.
*/
  std::string fullPath = knownIter->second ;
  char dirSep = CoinFindDirSeparator() ;
  std::string::size_type dirPos = fullPath.rfind(dirSep) ;
  std::string libName ;
  std::string dirName ;
  if (dirPos != std::string::npos) {
    libName = fullPath.substr(dirPos) ;
    dirName = fullPath.substr(0,dirPos) ;
    retval = pluginMgr_->unloadOneLib(libName,&dirName) ;
  } else {
    libName = fullPath ;
    retval = pluginMgr_->unloadOneLib(libName) ;
  }

  return (retval) ;
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
