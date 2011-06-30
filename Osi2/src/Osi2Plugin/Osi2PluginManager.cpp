
#include <cstring>
#include <cassert>
#include <string>
#include <iostream>

/*
  #include "Osi2Directory.hpp"
  #include "Osi2Path.hpp"
*/
#include "Osi2PluginManager.hpp"
#include "Osi2DynamicLibrary.hpp"
#include "Osi2ObjectAdapter.hpp"

#ifndef OSI2PLUGINDIR
# define OSI2DFLTPLUGINDIR "/usr/local/lib"
#else
# define OSI2DFLTPLUGINDIR OSI2PLUGINDIR
#endif

#if defined(Osi2_PLATFORM_MAC)
  static std::string dynamicLibraryExtension("dylib") ;
#elif defined(Osi2_PLATFORM_LINUX)
  static std::string dynamicLibraryExtension("so") ;
#elif defined(Osi2_PLATFORM_WINDOWS)
  static std::string dynamicLibraryExtension("dll") ;
#endif


// The registration params may be received from an external plugin so it is 
// crucial to validate it, because it was never subjected to our tests.

static bool isValid(const uint8_t *objectType,
		    const Osi2_RegisterParams *params)
{
  if (!objectType || !(*objectType))
     return false ;
  if (!params ||!params->createFunc || !params->destroyFunc)
    return false ;
  
  return true ;
}

// ---------------------------------------------------------------

int32_t Osi2PluginManager::registerObject (const uint8_t *objectType,
					   const Osi2_RegisterParams * params)
{
  // Check parameters
  if (!isValid(objectType, params))
    return -1 ;
 
  Osi2PluginManager &pm = getInstance() ;
  
  // Verify that versions match
  Osi2_PluginAPI_Version v = pm.platformServices_.version ;
  if (v.major != params->version.major)
    return -1 ;
    
  std::string key((const char *)objectType) ;
  std::cout << "Registering " << key << "." << std::endl ;
  // If it's a wild card registration just add it
  if (key == std::string("*"))
  {
    pm.wildCardVec_.push_back(*params) ;
    return 0 ;
  }
  
  // If item already exists in eactMatc fail (only one can handle)
  if (pm.exactMatchMap_.find(key) != pm.exactMatchMap_.end())    
    return -1 ;
  
  pm.exactMatchMap_[key] = *params ;
  return 0; 
}

// ---------------------------------------------------------------

Osi2PluginManager &Osi2PluginManager::getInstance()
{
  static Osi2PluginManager instance ;
  
  return instance ;
}


/*
 We're not going to implement loadAll functionality in the first round.
 Make this method into a guaranteed failure, so we'll catch it when it's
 called.
*/

int32_t Osi2PluginManager::loadAll (const std::string &pluginDirectory,
				    Osi2_InvokeServiceFunc func)
{
  assert(false) ;

  return (-1) ;
}

#if defined(OSI2_IMPLEMENT_LOADALL)

int32_t Osi2PluginManager::loadAll(const std::string &pluginDirectory,
				   Osi2_InvokeServiceFunc func)
{
  if (pluginDirectory.empty()) // Check that the path is non-empty.
    return -1 ;

  platformServices_.invokeService = func ;

  Path dir_path(pluginDirectory) ;
  if (!dir_path.exists() || !dir_path.isDirectory())
    return -1 ;

  Directory::Entry e ;
  Directory::Iterator di(dir_path) ;
  while (di.next(e))
  {
    Path full_path(dir_path + Path(e.path)) ;

    // Skip directories
    if (full_path.isDirectory())
      continue ;

    // Skip files with the wrong extension
    std::string ext = std::string(full_path.getExtension()) ;
    if (ext != dynamicLibraryExtension)
      continue ;

    // Ignore return value
    /*int32_t res = */ loadByPath(std::string(full_path)) ;
  }

  return 0 ;
}
#endif		// OSI2_IMPLEMENT_LOADALL

int32_t Osi2PluginManager::initializePlugin (Osi2_InitFunc initFunc)
{
  Osi2PluginManager &pm = Osi2PluginManager::getInstance() ;

  Osi2_ExitFunc exitFunc = initFunc(&pm.platformServices_) ;
  if (!exitFunc)
    return -1 ;
  
  // Store the exit func so it can be called when unloading this plugin
  pm.exitFuncVec_.push_back(exitFunc) ;
  return 0 ;
}
 
Osi2PluginManager::Osi2PluginManager()
  : inInitializePlugin_(false)
{
  dfltPluginDir_ = std::string(OSI2DFLTPLUGINDIR) ;
  platformServices_.version.major = 1 ;
  platformServices_.version.minor = 0 ;
  platformServices_.invokeService = NULL ; // can be populated during loadAll()
  platformServices_.registerObject = registerObject ;
}

Osi2PluginManager::~Osi2PluginManager ()
{
  // Just in case it wasn't called earlier
  shutdown() ;
}

int32_t Osi2PluginManager::shutdown()
{
  int32_t result = 0 ;
  for (ExitFuncVec::iterator func = exitFuncVec_.begin(); func != exitFuncVec_.end(); ++func)
  {
    try
    {
      result = (*func)() ;
    }
    catch (...)
    {
      result = -1 ;
    }
  }
  
  
  dynamicLibraryMap_.clear() ;
  exactMatchMap_.clear() ;
  wildCardVec_.clear() ;
  exitFuncVec_.clear() ;
  
  return result ;
}

int32_t Osi2PluginManager::loadByPath (const std::string &pluginPath)
{
    /*
      Until we implement Path.
    
      Path path(pluginPath) ;
    */
    std::string path = pluginPath ;
    
    /*
      STARTUP HACK
      Assume for now we won't have to deal with symbolic links.

    // Resolve symbolic links
    #ifndef WIN32 
    if (path.isSymbolicLink())
    {
      char buff[APR_PATH_MAX+1] ;
      int length = ::readlink(path, buff, APR_PATH_MAX) ;
      if (length < 0)
        return -1 ;
        
      path = std::string(buff, length) ;
    }
    #endif
    */
             
    // Don't load the same dynamic library twice
    if (dynamicLibraryMap_.find(std::string(path)) != dynamicLibraryMap_.end())
      return -1 ;

    /*
      STARTUP HACK
      Assume for now this isn't necessary.

    path.makeAbsolute() ;
    */

    std::string errorString ;
    Osi2DynamicLibrary *d = loadLibrary(std::string(path), errorString) ;
    if (!d) // not a dynamic library? 
      return -1 ;
                    
    // Get the initPlugin() function
    Osi2_InitFunc initFunc = (Osi2_InitFunc)(d->getSymbol("Osi2_initPlugin")) ;
    if (!initFunc) // dynamic library missing entry point?
      return -1 ;
    
    int32_t res = initializePlugin(initFunc) ;
    if (res < 0) // failed to initalize?
      return res ;
    
    return 0 ;
}


// ---------------------------------------------------------------

//template <typename T, typename U>
//T *Osi2PluginManager::createObject(const std::string & objectType, IObjectAdapter<T, U> & adapter)
//{
//  // "*" is not a valid object type
//  if (objectType == std::string("*"))
//    return NULL ;
//  
//  // Prepare object params
//  Osi2_ObjectParams np ;
//  np.objectType = objectType.c_str() ;
//  np.platformServices = &ServiceProvider::getInstance() ;
//
//  // Exact match found
//  if (exactMatchMap_.find(objectType) != exactMatchMap_.end())
//  {        
//    Osi2_RegisterParams & rp = exactMatchMap_[objectType] ;
//    IObject * object = createObject(rp, np, adapter) ;
//    if (object) // great, it worked
//      return object ;
//  }
//  
//  for (Size i = 0; i < wildCardVec_.size(); ++i)
//  {
//    Osi2_RegisterParams & rp = wildCardVec_[i] ;
//    IObject * object = createObject(rp, np, adapter) ;
//    if (object) // great, it worked
//    {
//      // promote registration to exactMatc_ 
//      // (but keep also the wild card registration for other object types)
//      int32_t res = registerObject(np.objectType, &rp) ;
//      if (res < 0)
//      {
//        Osi2_THROW << "Osi2PluginManager::createObject(" << np.objectType << "), registration failed" ;
//        delete object ;
//        return NULL ;
//      }
//      return object ;
//    }      
//  }
//
//  // Too bad no one can create this objectType
//  return NULL ;
//}


// ---------------------------------------------------------------


void *Osi2PluginManager::createObject(const std::string &objectType,
				      IObjectAdapter &adapter)
{
  // "*" is not a valid object type
  if (objectType == std::string("*"))
    return NULL ;
  
  // Prepare object params
  Osi2_ObjectParams np ;
  np.objectType = (const uint8_t *)objectType.c_str() ;
  np.platformServices = &platformServices_ ;

  // Exact match found
  if (exactMatchMap_.find(objectType) != exactMatchMap_.end())
  {        
    Osi2_RegisterParams & rp = exactMatchMap_[objectType] ;
    np.client = rp.client ;
    void *object = rp.createFunc(&np) ;
    if (object) // great, there is an exact match
    {
      // Adapt if necessary (wrap C objects using an adapter)
      if (rp.lang == Osi2_Plugin_C)
        object = adapter.adapt(object, rp.destroyFunc) ;
        
      return object; 
    }
  }
  
  // Try to find a wild card match
  for (size_t i = 0; i < wildCardVec_.size(); ++i)
  {
    Osi2_RegisterParams & rp = wildCardVec_[i] ;
    np.client = rp.client ;
    void *object = rp.createFunc(&np) ;
    if (object) // great, it worked
    {
      // Adapt if necessary (wrap C objects using an adapter)
      if (rp.lang == Osi2_Plugin_C)
        object = adapter.adapt(object, rp.destroyFunc) ;

      // promote registration to exactMatc_ 
      // (but keep also the wild card registration for other object types)
      int32_t res = registerObject(np.objectType, &rp) ;
      if (res < 0)
      {  
        // Serious framework should report or log it              
        rp.destroyFunc(object) ;
        return NULL ;
      }
      return object ;
    }      
  }

  // Too bad no one can create this objectType
  return NULL ;
}

Osi2DynamicLibrary *Osi2PluginManager::loadLibrary (const std::string &path,
						    std::string & errorString)
{
    Osi2DynamicLibrary *d = Osi2DynamicLibrary::load(path, errorString) ;
    if (!d) // not a dynamic library? 
      return NULL ;
    /*
      STARTUP HACK
      Assume for now we don't need to worry about absolute paths.

    // Add library to map, so it can be unloaded
    dynamicLibraryMap_[Path::makeAbsolute(path)] = boost::shared_ptr<DynamicLibrary>(d) ;
    */
    dynamicLibraryMap_[path] = d ;
    return (d) ;
}

const Osi2PluginManager::RegistrationMap &Osi2PluginManager::getRegistrationMap ()
{
  return (exactMatchMap_) ;
}

Osi2_PlatformServices &Osi2PluginManager::getPlatformServices ()
{
  return (platformServices_) ;
}
