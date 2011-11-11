/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  Based on and extended from original design and code by Gigi Sayfan published
  in five parts in Dr. Dobbs, starting November 2007.
*/
/*! \file Osi2PluginManager.cpp
    \brief Definition of methods for Osi2::PluginManager.
*/

#include <cstring>
#include <cassert>
#include <string>
#include <iostream>

#include "CoinHelperFunctions.hpp"

/*
  #include "Osi2Directory.hpp"
  #include "Osi2Path.hpp"
*/
#include "Osi2Config.h"
#include "Osi2nullptr.hpp"
#include "Osi2PluginManager.hpp"
#include "Osi2DynamicLibrary.hpp"
#include "Osi2ObjectAdapter.hpp"

#ifndef OSI2PLUGINDIR
# define OSI2DFLTPLUGINDIR "/usr/local/lib"
#else
# define OSI2DFLTPLUGINDIR OSI2PLUGINDIR
#endif

#if defined(OSI2PLATFORM_MAC)
static std::string dynamicLibraryExtension("dylib") ;
#elif defined(OSI2PLATFORM_LINUX)
static std::string dynamicLibraryExtension("so") ;
#elif defined(OSI2PLATFORM_WINDOWS)
static std::string dynamicLibraryExtension("dll") ;
#endif

using namespace Osi2 ;


/*
  Plugin manager constructor.
*/
PluginManager::PluginManager()
    : initialisingPlugin_(false),
      dfltHandler_(true),
      logLvl_(7)
{
    msgHandler_ = new CoinMessageHandler() ;
    msgs_ = PlugMgrMessages() ;
    msgHandler_->setLogLevel(logLvl_) ;
    msgHandler_->message(PLUGMGR_INIT, msgs_) << CoinMessageEol ;
    dfltPluginDir_ = std::string(OSI2DFLTPLUGINDIR) ;
    platformServices_.version_.major_ = 1 ;
    platformServices_.version_.minor_ = 0 ;
    platformServices_.dfltPluginDir_ =
        reinterpret_cast<const CharString*>(dfltPluginDir_.c_str()) ;
    // can be populated during loadAll()
    platformServices_.invokeService_ = nullptr ;
    platformServices_.registerObject_ = registerObject ;
}

/*
  Plugin manager destructor

  Call shutdown to close out the plugins, then destroy the pieces of ourself.
*/
PluginManager::~PluginManager ()
{
    // Just in case it wasn't called earlier
    shutdown() ;
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
  There's no particular reason to trust a plugin, so we need to do some
  validation of the parameters it gives to the plugin manager when registering
  an API.

  \parm Registration string supplied by plugin
  \parm Registration parameter block supplied by plugin

  \returns Pointer to the DynamicLibrary struct for this library, or
  	   null on error.
*/

DynamicLibrary *PluginManager::validateRegParams (const CharString *apiStr,
        const RegisterParams *params) const
{
    bool retval = true ;
    std::string errStr = "" ;
    DynamicLibrary *dynLib = nullptr ;

    // The API to be registered must have a name
    if (apiStr == nullptr || !(*apiStr)) {
        errStr += "; bad API ID string" ;
        retval = false ;
    }

    // If we're missing the parameter block, nothing more to say.
    if (params == nullptr) {
        errStr += "; no registration parameter block" ;
        retval = false ;
    } else {
        /*
          The pluginID must be valid. If we're initialising the library, it will
          not appear in dynamicLibraryMap.
        */
        if (initialisingPlugin_) {
            if (libInInit_ != params->pluginID_) {
                errStr += "; bad plugin library ID" ;
                retval = false ;
            } else {
                dynLib = libInInit_ ;
            }
        } else {
            DynamicLibraryMap::const_iterator dlmIter ;
            for (dlmIter = dynamicLibraryMap_.begin() ;
                    dlmIter != dynamicLibraryMap_.end() ;
                    dlmIter++) {
                if (params->pluginID_ == dlmIter->second.dynLib_) break ;
            }
            if (dlmIter == dynamicLibraryMap_.end()) {
                errStr += "; bad plugin library ID" ;
                retval = false ;
            } else {
                dynLib = dlmIter->second.dynLib_ ;
            }
        }
        // There must be a constructor
        if (params->createFunc_ == nullptr) {
            errStr += "; missing constructor" ;
            retval = false ;
        }
        // There must be a destructor
        if (params->destroyFunc_ == nullptr) {
            errStr += "; missing destructor" ;
            retval = false ;
        }
        // The major version must match
        PluginAPIVersion ver = platformServices_.version_ ;
        if (ver.major_ != params->version_.major_) {
            msgHandler_->message(PLUGMGR_BADVER, msgs_)
                    << params->version_.major_ << ver.major_ << CoinMessageEol ;
            errStr += "; version mismatch" ;
            retval = false ;
        }
    }

    if (!retval) {
        errStr = errStr.substr(2) ;
        msgHandler_->message(PLUGMGR_APIBADPARM, msgs_) << errStr << CoinMessageEol ;
    }

    return (dynLib) ;
}


/*
  Check for an entry that matches key and libID. Returns an iterator for the
  entry, if it exists, else regMap.end(). A libID of 0 is a wildcard.
*/
PluginManager::RegistrationMap::iterator
PluginManager::apiEntryExists (RegistrationMap &regMap,
                               const std::string &key,
                               PluginUniqueID libID)
{
    typedef RegistrationMap::iterator RMCI ;
    /*
      See if we find anything. For the case where there's no entry at all
      for this API, iterPair.first will be regMap.end(). For the case where
      the libID is a wildCard (0), we're perfectly happy to use the first
      entry found. In either case, we can simply return iterPair.first.
    */
    std::pair<RMCI, RMCI> iterPair = regMap.equal_range(key) ;
    if (libID == 0 || iterPair.first == regMap.end()) return (iterPair.first) ;
    /*
      There's at least one entry matching the API (key). Check to see if any of
      them are from the same plugin library.
    */
    for (RMCI iter = iterPair.first ; iter != iterPair.second ; iter++) {
        RegisterParams parms = iter->second ;
        if (parms.pluginID_ == libID) return (iter) ;
    }

    return (regMap.end()) ;
}



/*
  This method is used by the plugin to register the APIs it supports. A pointer
  to this method is passed to the plugin in a PlatformServices object.

  Returns 0 for success, -1 for failure.
*/
int32_t PluginManager::registerObject (const CharString *apiStr,
                                       const RegisterParams *params)
{
    PluginManager &pm = getInstance() ;

    // Validate the parameter block
    DynamicLibrary *dynLib = pm.validateRegParams(apiStr, params) ;
    if (dynLib == nullptr) return (-1) ;
    /*
      If the registration is a wild card, add it to the wild card vector. If the
      registration is for a specific API, check before adding to the exact match
      vector --- duplicates are not allowed.

      If this call comes while we're initialising a plugin, add the APIs to
      temporary vectors for merge if initialisation is successful.

      Note that we cannot just store a pointer to the RegisterParams block;
      there's no telling how the plugin is handling it and we don't get ownership.

      \todo
      Note that the same API can be registered by multiple plugin libraries, so
      checking for duplicate entries is a bit more work. If we're guarding against
      a malfunctioning plugin making a duplicate registration, we should also check
      for duplicate entries when inserting into the wildcard vector.
      -- lh, 111013 --
    */
    const std::string key((const char *) apiStr) ;
    int retval = 0 ;

    if (key == std::string("*")) {
        if (pm.initialisingPlugin_) {
            pm.tmpWildCardVec_.push_back(*params) ;
        } else {
            pm.wildCardVec_.push_back(*params) ;
        }
    } else {
        RegistrationMap::const_iterator rmIter ;
        if (pm.initialisingPlugin_) {
            rmIter = pm.apiEntryExists(pm.exactMatchMap_, key, dynLib) ;
            if (rmIter != pm.exactMatchMap_.end()) retval = -1 ;
        } else {
            rmIter = pm.apiEntryExists(pm.tmpExactMatchMap_, key, dynLib) ;
            if (rmIter != pm.tmpExactMatchMap_.end()) retval = -1 ;
        }
        if (retval) {
            pm.msgHandler_->message(PLUGMGR_APIREGDUP, pm.msgs_)
                    << key << CoinMessageEol ;
        } else {
            RegistrationMap::value_type tmp(key, *params) ;
            if (pm.initialisingPlugin_) {
                pm.tmpExactMatchMap_.insert(tmp) ;
            } else {
                pm.exactMatchMap_.insert(tmp) ;
            }
        }
    }
    if (!retval)
        pm.msgHandler_->message(PLUGMGR_APIREGOK, pm.msgs_)
                << key << dynLib->getLibPath() << CoinMessageEol ;
    return (retval) ;
}

/*
  Instantiate a single instance of the manager.
*/
PluginManager &PluginManager::getInstance()
{
    static PluginManager instance ;

    return (instance) ;
}


/*
  Load a plugin given a full path to the plugin.

  If uniqueID is supplied, it will be loaded with the PluginUniqueID assigned
  to the library.

  Returns:
    -3: library failed to initialise
    -2: failed to find initFunc
    -1: library failed to load
     0: library loaded and initialised without error
     1: library is already loaded

  \todo Implement platform-independent path handling.
  \todo Implement resolution of symbolic links.
  \todo Implement conversion to absolute path.
*/
int PluginManager::loadOneLib (const std::string &lib, const std::string *dir,
                               PluginUniqueID *uniqueID)
{
    if (uniqueID != 0) (*uniqueID) = 0 ;
    /*
      Construct the full path for the library.

      Should this be converted to a standardised absolute path? That'd
      avoid issues with different ways of specifying the same library.
    */
    std::string fullPath ;
    if (dir == nullptr || (dir->compare("") == 0)) {
        fullPath += getDfltPluginDir() ;
    } else {
        fullPath += *dir ;
    }
    char dirSep = CoinFindDirSeparator() ;
    fullPath += dirSep + lib ;
    /*
      Is this library already loaded? If so, don't do it again.
    */
    if (libPathToIDMap_.find(fullPath) != libPathToIDMap_.end()) {
        msgHandler_->message(PLUGMGR_LIBLDDUP, msgs_)
                << fullPath << CoinMessageEol ;
        return (1) ;
    }
    /*
      Attempt to load the library. A null return indicates failure. Report the
      problem and return an error.
    */
    std::string errStr ;
    DynamicLibrary *dynLib = DynamicLibrary::load(fullPath, errStr) ;
    if (dynLib == nullptr) {
        msgHandler_->message(PLUGMGR_LIBLDFAIL, msgs_)
                << fullPath << errStr << CoinMessageEol ;
        return (-1) ;
    }
    /*
      Find the initialisation function "initPlugin". If this entry point
      is missing from the library, we're in trouble.
    */
    InitFunc initFunc = (InitFunc)(dynLib->getSymbol("initPlugin", errStr)) ;
    if (initFunc == nullptr) {
        msgHandler_->message(PLUGMGR_SYMLDFAIL, msgs_)
                << "function" << "initPlugin" << fullPath << errStr << CoinMessageEol ;
        return (-2) ;
    }
    /*
      Invoke the initialisation function, which will (in the typical case)
      trigger the registration of the various APIs implemented in this
      plugin library. We should get back the exit function for the library.

      Set initialisingPlugin_ to true so that the APIs will be registered
      into the temporary wildcard and exact match vectors. If initialisation
      is successful, we'll transfer them to the permanent vectors.
    */
    initialisingPlugin_ = true ;
    libInInit_ = dynLib ;
    tmpExactMatchMap_.clear() ;
    tmpWildCardVec_.clear() ;
    platformServices_.dfltPluginDir_ =
        reinterpret_cast<const CharString*>(dfltPluginDir_.c_str()) ;
    platformServices_.pluginID_ = dynLib ;
    ExitFunc exitFunc = initFunc(&platformServices_) ;
    if (exitFunc == nullptr) {
        msgHandler_->message(PLUGMGR_LIBINITFAIL, msgs_)
                << fullPath << CoinMessageEol ;
        delete dynLib ;
        initialisingPlugin_ = false ;
        return (-3) ;
    }
    msgHandler_->message(PLUGMGR_LIBINITOK, msgs_)
            << fullPath << CoinMessageEol ;
    /*
      We have happiness: the library is loaded and initialised. Do the
      bookkeeping.  Enter the library in the library map. Add the exit function
      to the vector of exit functions, and copy information from the temporary
      wildcard and exact match vectors to the permanent vectors.

      \todo
      How paranoid do we want to be? Given that we check entries for uniqueness
      as they're going into tmpExactMatchMap_, and that you can't load the same
      lib twice, should we check tmpExactMatchMap_ against exactMatchMap_?
      -- lh, 111013 --
    */
    libPathToIDMap_[fullPath] = dynLib ;
    DynLibInfo &info = dynamicLibraryMap_[dynLib] ;
    info.dynLib_ = dynLib ;
    info.ctrlObj_ = platformServices_.ctrlObj_ ;
    info.exitFunc_ = exitFunc ;
    exactMatchMap_.insert(tmpExactMatchMap_.begin(), tmpExactMatchMap_.end()) ;
    tmpExactMatchMap_.clear() ;
    wildCardVec_.insert(wildCardVec_.end(),
                        tmpWildCardVec_.begin(), tmpWildCardVec_.end()) ;
    tmpWildCardVec_.clear() ;
    initialisingPlugin_ = false ;
    if (uniqueID != 0) (*uniqueID) = dynLib ;

    msgHandler_->message(PLUGMGR_LIBLDOK, msgs_) << fullPath << CoinMessageEol ;

    return (0) ;
}


/*
 We're not going to implement loadAll functionality in the first round.
 Make this method into a guaranteed failure, so we'll catch it when it's
 called.
*/

int PluginManager::loadAllLibs (const std::string &libDir,
                                InvokeServiceFunc func)
{
    assert(false) ;

    return (-1) ;
}

#if defined(OSI2_IMPLEMENT_LOADALL)

int32_t PluginManager::loadAll(const std::string &libDir,
                               InvokeServiceFunc func)
{
    if (libDir.empty()) // Check that the path is non-empty.
        return -1 ;

    platformServices_.invokeService = func ;

    Path dir_path(libDir) ;
    if (!dir_path.exists() || !dir_path.isDirectory())
        return -1 ;

    Directory::Entry e ;
    Directory::Iterator di(dir_path) ;
    while (di.next(e)) {
        Path full_path(dir_path + Path(e.path)) ;

        // Skip directories
        if (full_path.isDirectory())
            continue ;

        // Skip files with the wrong extension
        std::string ext = std::string(full_path.getExtension()) ;
        if (ext != dynamicLibraryExtension)
            continue ;

        // Ignore return value
        /*int32_t res = */
        loadOneLib(std::string(full_path)) ;
    }

    return 0 ;
}
#endif		// OSI2_IMPLEMENT_LOADALL


/*
  Unload a single library specified by name. The name must exactly match the
  name used to load the library. Issue a warning if the library isn't loaded.

  Returns:  1 if the library isn't loaded
	    0 if the library unloads successfully
	   -1 exit function failed
*/
int PluginManager::unloadOneLib (const std::string &lib,
                                 const std::string *dir)
{
    int result = 0 ;

    /*
      If no directory is specified, use the default plugin directory.
    */
    std::string fullPath ;
    if (dir == nullptr || (dir->compare("") == 0)) {
        fullPath += getDfltPluginDir() ;
    } else {
        fullPath += *dir ;
    }
    char dirSep = CoinFindDirSeparator() ;
    fullPath += dirSep + lib ;
    /*
      Find the entry for the library. Warn the user if the library is
      not loaded.
    */
    LibPathToIDMap::iterator lpiIter = libPathToIDMap_.find(fullPath) ;
    if (lpiIter == libPathToIDMap_.end()) {
        msgHandler_->message(PLUGMGR_LIBNOTFOUND, msgs_)
                << fullPath << CoinMessageEol ;
        return (1) ;
    }
    DynamicLibraryMap::iterator dlmIter =
        dynamicLibraryMap_.find(lpiIter->second) ;
    /*
      Step through the exact match map and remove any entries that are
      registered to this library. Regrettably, erase for a map is defined
      to invalidate the iterator pointing to the element. But not iterators
      pointing to other elements, so we need to advance prior to erasing.
    */
    DynLibInfo &libInfo = dlmIter->second ;
    DynamicLibrary *dynLib = libInfo.dynLib_ ;
    RegistrationMap::iterator rmIter = exactMatchMap_.begin() ;
    while (rmIter != exactMatchMap_.end()) {
        RegisterParams &regParms = rmIter->second ;
        if (regParms.pluginID_ == dynLib) {
            RegistrationMap::iterator tmpIter = rmIter ;
            rmIter++ ;
            msgHandler_->message(PLUGMGR_APIUNREG, msgs_)
                    << tmpIter->first << dynLib->getLibPath() << CoinMessageEol ;
            exactMatchMap_.erase(tmpIter) ;
        } else {
            rmIter++ ;
        }
    }
    /*
      See if there's an entry in the wildcard vector. Vectors don't have the
      same problem as maps (erase returns a valid iterator), but there's only
      one entry so it's irrelevant.
    */
    for (RegistrationVec::iterator rvIter = wildCardVec_.begin() ;
            rvIter != wildCardVec_.end() ;
            rvIter++) {
        RegisterParams &regParms = *rvIter ;
        if (regParms.pluginID_ == dynLib) {
            msgHandler_->message(PLUGMGR_APIUNREG, msgs_)
                    << "wildcard" << dynLib->getLibPath() << CoinMessageEol ;
            wildCardVec_.erase(rvIter) ;
            break ;
        }
    }
    /*
      Execute the exit function for the library.
    */
    bool threwError = false ;
    ExitFunc func = libInfo.exitFunc_ ;
    platformServices_.pluginID_ = dynLib ;
    platformServices_.ctrlObj_ = libInfo.ctrlObj_ ;
    try {
        result = (*func)(&platformServices_) ;
    } catch (...) {
        threwError = true ;
    }
    if (threwError || result != 0) {
        msgHandler_->message(PLUGMGR_LIBEXITFAIL, msgs_)
                << dynLib->getLibPath() << CoinMessageEol ;
        result = -1 ;
    } else {
        msgHandler_->message(PLUGMGR_LIBEXITOK, msgs_)
                << dynLib->getLibPath() << CoinMessageEol ;
    }
    /*
      Unload the library and remove the map entry.
    */
    msgHandler_->message(PLUGMGR_LIBCLOSE, msgs_)
            << dynLib->getLibPath() << CoinMessageEol ;
    delete dynLib ;
    dynamicLibraryMap_.erase(dlmIter) ;
    libPathToIDMap_.erase(lpiIter) ;

    return (result) ;
}



/*
  Execute the exit function for each library, then clear out the maps in the
  manager. Executing the destructor for the DynamicLibrary object will unload
  the library.

  \todo: The exit functions can throw? Why isn't there a catch block for all
	 the others (init function, etc.)
*/
int PluginManager::shutdown()
{
    int overallResult = 0 ;

    for (DynamicLibraryMap::iterator dlmIter = dynamicLibraryMap_.begin() ;
            dlmIter != dynamicLibraryMap_.end() ;
            dlmIter++) {
        int result = 0 ;
        bool threwError = false ;
        ExitFunc func = dlmIter->second.exitFunc_ ;
        DynamicLibrary *dynLib = dlmIter->second.dynLib_ ;
        platformServices_.pluginID_ = dynLib ;
        platformServices_.ctrlObj_ = dlmIter->second.ctrlObj_ ;
        try {
            result = (*func)(&platformServices_) ;
        } catch (...) {
            threwError = true ;
        }
        if (threwError || result != 0) {
            msgHandler_->message(PLUGMGR_LIBEXITFAIL, msgs_)
                    << dynLib->getLibPath() << CoinMessageEol ;
            overallResult-- ;
        } else {
            msgHandler_->message(PLUGMGR_LIBEXITOK, msgs_)
                    << dynLib->getLibPath() << CoinMessageEol ;
        }
    }
    /*
      Clear out the maps. Before we clear dynamicLibraryMap_, go through and
      delete the DynamicLibrary objects; the destructor will unload the library.
      The other vectors hold actual objects rather than pointers.
    */
    for (DynamicLibraryMap::iterator dlmIter = dynamicLibraryMap_.begin() ;
            dlmIter != dynamicLibraryMap_.end() ;
            dlmIter++) {
        DynamicLibrary *dynLib = dlmIter->second.dynLib_ ;
        msgHandler_->message(PLUGMGR_LIBCLOSE, msgs_)
                << dynLib->getLibPath() << CoinMessageEol ;
        delete dynLib ;
    }
    libPathToIDMap_.clear() ;
    dynamicLibraryMap_.clear() ;
    exactMatchMap_.clear() ;
    wildCardVec_.clear() ;

    return (overallResult) ;
}

/*
  Replace the current handler with a new handler. The current handler may or
  may not be our responsibility. If newHandler is null, create a default
  handler. (We can't be without one; the code isn't prepared for that.)
*/
void PluginManager::setMsgHandler (CoinMessageHandler *newHandler)
{
    if (dfltHandler_) {
        delete msgHandler_ ;
        msgHandler_ = nullptr ;
    }
    if (newHandler) {
        dfltHandler_ = false ;
        msgHandler_ = newHandler ;
    } else {
        dfltHandler_ = true ;
        msgHandler_ = new CoinMessageHandler() ;
        msgHandler_->setLogLevel(logLvl_) ;
    }
}


// ---------------------------------------------------------------

/*
  Construct an ObjectParams block to pass to the plugin.

  This method hides the details of constructing the object creation parameters.
  In particular, it hides the business of finding the correct library state
  object.
*/
ObjectParams *PluginManager::buildObjectParams (const std::string apiStr,
        const RegisterParams &rp)
{
    ObjectParams *objParms = new ObjectParams() ;
    objParms->apiStr_ = reinterpret_cast<const CharString *>(apiStr.c_str()) ;

    DynamicLibraryMap::iterator dlmIter =
        dynamicLibraryMap_.find(rp.pluginID_) ;
    DynLibInfo &dynLib = dlmIter->second ;
    platformServices_.pluginID_ = rp.pluginID_ ;
    platformServices_.ctrlObj_ = dynLib.ctrlObj_ ;
    objParms->platformServices_ = &platformServices_ ;
    objParms->ctrlObj_ = rp.ctrlObj_ ;

    return (objParms) ;
}


void *PluginManager::createObject (const std::string &apiStr,
                                   PluginUniqueID &libID,
                                   IObjectAdapter &adapter)
{
    /*
      "*" is not a valid object type --- some qualification is needed.
    */
    if (apiStr == std::string("*")) {
        msgHandler_->message(PLUGMGR_APICREATEFAIL, msgs_)
                << apiStr << "wildcard is invalid for createObject" << CoinMessageEol ;
        return (nullptr) ;
    }
    /*
      Check for an exact (string) match. If so, add the plugin's management object
      to the parameter block and ask for an object. If we're successful, we need
      one last step for a C plugin --- wrap it for C++ use.
    */
    RegistrationMap::iterator apiIter = apiEntryExists(exactMatchMap_,
                                        apiStr, libID) ;
    if (apiIter != exactMatchMap_.end()) {
        RegisterParams &rp = apiIter->second ;
        ObjectParams *objParms = buildObjectParams(apiStr, rp) ;
        void *object = rp.createFunc_(objParms) ;
        delete objParms ;
        if (object) {
            msgHandler_->message(PLUGMGR_APICREATEOK, msgs_)
                    << apiStr << "exact" << CoinMessageEol ;
	    if (libID == 0) libID = rp.pluginID_ ;
            if (rp.lang_ == Plugin_C)
                object = adapter.adapt(object, rp.destroyFunc_) ;
            return (object) ;
        }
    }
    /*
      No exact match. Try for a wildcard match. If some plugin volunteers an
      object, register it. As with exact match, respect a restriction to a
      particular plugin library.

      Surely we could do better here than just blindly calling create functions?
      Maybe not. It's not really all that different from having a separate `Can
      you do this?' method, given the feedback where the manager adds the requested
      string to the exactMatchMap via the registerObject method. Note that the
      entry in the wildCardVec is not removed.
      -- lh, 110913 --

      Do we need to tweak the registration parameter block before registering the
      `exact match' object?  -- lh, 111005 --

      If we wrap a C plugin object with an adapter, shouldn't we delete the
      adapter object and give the adapter's delete the responsibility for
      calling the proper destroyFunc?   -- lh, 111013 --
    */
    for (size_t i = 0 ; i < wildCardVec_.size() ; ++i) {
        RegisterParams &rp = wildCardVec_[i] ;
        if (libID && rp.pluginID_ != libID) continue ;
        ObjectParams *objParms = buildObjectParams(apiStr, rp) ;
        void *object = rp.createFunc_(objParms) ;
        if (object) {
            msgHandler_->message(PLUGMGR_APICREATEOK, msgs_)
                    << apiStr << "wildcard" << CoinMessageEol ;
	    if (libID == 0) libID = rp.pluginID_ ;
            if (rp.lang_ == Plugin_C)
                object = adapter.adapt(object, rp.destroyFunc_) ;
            int32_t res = registerObject(objParms->apiStr_, &rp) ;
            if (res < 0) {
                rp.destroyFunc_(object, objParms) ;
                object = nullptr ;
            }
        }
        delete objParms ;
        return (object) ;
    }
    /*
      No plugin volunteered. We can't create this object.
    */
    msgHandler_->message(PLUGMGR_APICREATEFAIL, msgs_)
            << apiStr << "no capable plugin" << CoinMessageEol ;
    return (nullptr) ;
}

#ifdef UNDEFINED

/*
  Presumably this avoids requiring the client to cast to the appropriate type.
  Might be worth exploring.  -- lh, 110929 --
*/
template <typename T, typename U>
T *PluginManager::createObject(const std::string & apiStr, IObjectAdapter<T, U> & adapter)
{
    // "*" is not a valid object type
    if (apiStr == std::string("*"))
        return (nullptr) ;

    // Prepare object params
    ObjectParams np ;
    np.apiStr = apiStr.c_str() ;
    np.platformServices = &ServiceProvider::getInstance() ;

    // Exact match found
    if (exactMatchMap_.find(apiStr) != exactMatchMap_.end()) {
        RegisterParams & rp = exactMatchMap_[apiStr] ;
        IObject * object = createObject(rp, np, adapter) ;
        if (object) // great, it worked
            return object ;
    }

    for (Size i = 0; i < wildCardVec_.size(); ++i) {
        RegisterParams & rp = wildCardVec_[i] ;
        IObject * object = createObject(rp, np, adapter) ;
        if (object) { // great, it worked
            // promote registration to exactMatc_
            // (but keep also the wild card registration for other object types)
            int32_t res = registerObject(np.apiStr, &rp) ;
            if (res < 0) {
                THROW << "PluginManager::createObject(" << np.apiStr << "), registration failed" ;
                delete object ;
                return (nullptr) ;
            }
            return object ;
        }
    }

    // Too bad no one can create this objectType
    return (nullptr) ;
}
#endif


/*
  Destroy an object.

  Given that anything handed out by this manager is a C++ object, the client
  can invoke standard C++ delete and the destructor will do its work. But
  suppose that the plugin wants to do more than just invoke the destructor?
  This provides a way to invoke an arbitrary function. The only requirement is
  that the specified object be destroyed as part of the execution.
*/
int PluginManager::destroyObject (const std::string &apiStr,
                                  PluginUniqueID libID, void *victim)
{
    int result = 0 ;
    RegistrationMap::iterator apiIter = apiEntryExists(exactMatchMap_,
                                        apiStr, libID) ;
    if (apiIter == exactMatchMap_.end()) {
        msgHandler_->message(PLUGMGR_APIDELFAIL, msgs_)
                << apiStr << "no such API" << CoinMessageEol ;
        result = -1 ;
    } else {
        RegisterParams &rp = apiIter->second ;
        ObjectParams *objParms = buildObjectParams(apiStr, rp) ;
        result = rp.destroyFunc_(victim, objParms) ;
        if (result < 0) {
            msgHandler_->message(PLUGMGR_APIDELFAIL, msgs_)
                    << apiStr << "DestroyFunc failed" << CoinMessageEol ;
        }
    }
    if (result >= 0)
        msgHandler_->message(PLUGMGR_APIDELOK, msgs_) << apiStr << CoinMessageEol ;

    return (result) ;
}


PlatformServices &PluginManager::getPlatformServices ()
{
    return (platformServices_) ;
}

/*
  Find the full path for the library specified by libID.
*/
std::string PluginManager::getLibPath (PluginUniqueID libID)
{
    typedef LibPathToIDMap::const_iterator LPTIMI ;
    /*
      Step through the map and find the matching entry.
    */
    for (LPTIMI iter = libPathToIDMap_.begin() ;
         iter != libPathToIDMap_.end() ; iter++) {
        if (iter->second == libID) return (iter->first) ;
    }
    return ("<library not loaded>") ;
}

