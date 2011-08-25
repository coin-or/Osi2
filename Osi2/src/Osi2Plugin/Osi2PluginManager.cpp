
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

#if defined(OSI2PLATFORM_MAC)
  static std::string dynamicLibraryExtension("dylib") ;
#elif defined(OSI2PLATFORM_LINUX)
  static std::string dynamicLibraryExtension("so") ;
#elif defined(OSI2PLATFORM_WINDOWS)
  static std::string dynamicLibraryExtension("dll") ;
#endif

using namespace Osi2 ;

/*
  There's no particular reason to trust a plugin, so we need to do some
  validation of the parameters it gives to the plugin manager.

  Open question: This is Sayfan's notion of validation. It should be improved
  as we add parameters.  -- lh, 110825 --

  Why is this static?  -- lh, 110825 --
*/

static bool isValid (const uint8_t *objectType,
		     const RegisterParams *params)
{
  if (!objectType || !(*objectType))
     return (false) ;
  if (!params || !params->createFunc || !params->destroyFunc)
    return (false) ;
  
  return (true) ;
}


int32_t PluginManager::registerObject (const uint8_t *objectType,
				       const RegisterParams *params)
{
  // Check parameters
  if (!isValid(objectType, params))
    return (-1) ;
 
  PluginManager &pm = getInstance() ;
  
  // Verify that versions match
  PluginAPIVersion v = pm.platformServices_.version ;
  if (v.major != params->version.major)
    return (-1) ;
    
  std::string key((const char *)objectType) ;
  std::cout << "Registering " << key << "." << std::endl ;

  // If it's a wild card registration just add it
  if (key == std::string("*"))
  {
    pm.wildCardVec_.push_back(*params) ;
    return (0) ;
  }
  
  // If item already exists in exactMatch, fail. (Only one can handle.)
  if (pm.exactMatchMap_.find(key) != pm.exactMatchMap_.end())    
    return (-1) ;
  
  pm.exactMatchMap_[key] = *params ;
  return (0) ; 
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
 We're not going to implement loadAll functionality in the first round.
 Make this method into a guaranteed failure, so we'll catch it when it's
 called.
*/

int32_t PluginManager::loadAll (const std::string &pluginDirectory,
				    InvokeServiceFunc func)
{
  assert(false) ;

  return (-1) ;
}

#if defined(OSI2_IMPLEMENT_LOADALL)

int32_t PluginManager::loadAll(const std::string &pluginDirectory,
				   InvokeServiceFunc func)
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

int32_t PluginManager::initializePlugin (InitFunc initFunc)
{
  PluginManager &pm = PluginManager::getInstance() ;

  ExitFunc exitFunc = initFunc(&pm.platformServices_) ;
  if (!exitFunc)
    return -1 ;
  
  // Store the exit func so it can be called when unloading this plugin
  pm.exitFuncVec_.push_back(exitFunc) ;
  return 0 ;
}
 
PluginManager::PluginManager()
  : inInitializePlugin_(false)
{
  dfltPluginDir_ = std::string(OSI2DFLTPLUGINDIR) ;
  platformServices_.version.major = 1 ;
  platformServices_.version.minor = 0 ;
  platformServices_.dfltPluginDir =
    reinterpret_cast<const uint8_t*>(dfltPluginDir_.c_str()) ;
  platformServices_.invokeService = NULL ; // can be populated during loadAll()
  platformServices_.registerObject = registerObject ;
}

PluginManager::~PluginManager ()
{
  // Just in case it wasn't called earlier
  shutdown() ;
}

int32_t PluginManager::shutdown()
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

int32_t PluginManager::loadByPath (const std::string &pluginPath)
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
    DynamicLibrary *d = loadLibrary(std::string(path), errorString) ;
    if (!d) // not a dynamic library? 
      return -1 ;
                    
    // Get the initPlugin() function
    InitFunc initFunc = (InitFunc)(d->getSymbol("initPlugin")) ;
    if (!initFunc) // dynamic library missing entry point?
      return -1 ;
    
    int32_t res = initializePlugin(initFunc) ;
    if (res < 0) // failed to initalize?
      return res ;
    
    return 0 ;
}


// ---------------------------------------------------------------

//template <typename T, typename U>
//T *PluginManager::createObject(const std::string & objectType, IObjectAdapter<T, U> & adapter)
//{
//  // "*" is not a valid object type
//  if (objectType == std::string("*"))
//    return NULL ;
//  
//  // Prepare object params
//  ObjectParams np ;
//  np.objectType = objectType.c_str() ;
//  np.platformServices = &ServiceProvider::getInstance() ;
//
//  // Exact match found
//  if (exactMatchMap_.find(objectType) != exactMatchMap_.end())
//  {        
//    RegisterParams & rp = exactMatchMap_[objectType] ;
//    IObject * object = createObject(rp, np, adapter) ;
//    if (object) // great, it worked
//      return object ;
//  }
//  
//  for (Size i = 0; i < wildCardVec_.size(); ++i)
//  {
//    RegisterParams & rp = wildCardVec_[i] ;
//    IObject * object = createObject(rp, np, adapter) ;
//    if (object) // great, it worked
//    {
//      // promote registration to exactMatc_ 
//      // (but keep also the wild card registration for other object types)
//      int32_t res = registerObject(np.objectType, &rp) ;
//      if (res < 0)
//      {
//        THROW << "PluginManager::createObject(" << np.objectType << "), registration failed" ;
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


void *PluginManager::createObject(const std::string &objectType,
				      IObjectAdapter &adapter)
{
  // "*" is not a valid object type
  if (objectType == std::string("*"))
    return NULL ;
  
  // Prepare object params
  ObjectParams np ;
  np.objectType = (const uint8_t *)objectType.c_str() ;
  np.platformServices = &platformServices_ ;

  // Exact match found
  if (exactMatchMap_.find(objectType) != exactMatchMap_.end())
  {        
    RegisterParams & rp = exactMatchMap_[objectType] ;
    np.client = rp.client ;
    void *object = rp.createFunc(&np) ;
    if (object) // great, there is an exact match
    {
      // Adapt if necessary (wrap C objects using an adapter)
      if (rp.lang == Plugin_C)
        object = adapter.adapt(object, rp.destroyFunc) ;
        
      return object; 
    }
  }
  
  // Try to find a wild card match
  for (size_t i = 0; i < wildCardVec_.size(); ++i)
  {
    RegisterParams & rp = wildCardVec_[i] ;
    np.client = rp.client ;
    void *object = rp.createFunc(&np) ;
    if (object) // great, it worked
    {
      // Adapt if necessary (wrap C objects using an adapter)
      if (rp.lang == Plugin_C)
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

DynamicLibrary *PluginManager::loadLibrary (const std::string &path,
						    std::string & errorString)
{
    DynamicLibrary *d = DynamicLibrary::load(path, errorString) ;
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

const PluginManager::RegistrationMap &PluginManager::getRegistrationMap ()
{
  return (exactMatchMap_) ;
}

PlatformServices &PluginManager::getPlatformServices ()
{
  return (platformServices_) ;
}


