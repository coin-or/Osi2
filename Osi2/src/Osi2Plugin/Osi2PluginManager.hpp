
#ifndef OSI2PLUGINMANAGER_HPP
#define OSI2PLUGINMANAGER_HPP

#include <vector>
#include <map>
#include "Osi2PlugMgrMessages.hpp"
#include "Osi2Plugin.hpp"


namespace Osi2 {

class DynamicLibrary ;
struct IObjectAdapter ;

/*! \brief Plugin library manager

  This class provides generic support for loading and unloading plugin
  libraries. As libraries are loaded, they register the APIs that they support
  with the manager. Clients can then ask for objects which support a registered
  API. APIs are identified by strings.

  The act of loading a plugin library is an atomic transaction: The library
  must be found, loaded, and successfully complete its initialisation function
  before registration information is copied into the manager's client-facing
  data structures.

  There is a single instance of the plugin manager (declared as a static
  object private to #getInstance).
*/
  
class PluginManager
{

public:

  /*! \brief Get a reference to the plugin manager

    Returns a reference to the single instance of the plugin manager.
  */
  static PluginManager &getInstance() ;

  /*! \name Public plugin library management methods

    Methods to load and unload plugin libraries.
  */
  //@{

  /*! \brief Shut down the plugin manager

    Will invoke the exit method for all plugins, then shut down the plugin
    manager.
  */
  int32_t shutdown() ;

  /// Get the default plugin directory
  inline std::string getDfltPluginDir() { return (dfltPluginDir_) ; }

  /// Set the default plugin directory
  inline void setDfltPluginDir(std::string dfltDir)
  { dfltPluginDir_ = dfltDir ; }

  /*! \brief Scan a directory and load all plugins

    Currently unimplemented until we can create platform-independent file
    system support.
  */
  int32_t loadAll(const std::string &pluginDirectory,
		  InvokeServiceFunc func = NULL) ;

  /*! \brief Load the specified plugin

    Should deal with things like symbolic links but that function is
    currently disabled pending file system support.
  */
  int32_t loadByPath(const std::string & path) ;

  /// Get the services provided by the plugin manager
  PlatformServices &getPlatformServices() ;

  //@}

  /*! \name Factory methods

    Methods to create and destroy objects supported by plugins.
  */
  //@{

  /*! \brief Invoked by application to create an object

    Plugins register the objects they can create. This method is invoked by the
    application to request an object. The pointer returned must be cast to
    the appropriate type.

    An explanation of adapter is needed somewhere.
  */
  void *createObject(const std::string &objectType, IObjectAdapter &adapter) ;

  //@}


  /*! \brief Initialise a plugin

    Invokes the plugin's initialisation method
  */
  static int32_t initializePlugin(InitFunc initFunc) ;

private:
  /*! \brief Register an object type with the plugin manager

    Invoked by plugins to register the objects they can create.
  */
  static int32_t registerObject(const CharString *nodeType,
                                const RegisterParams *params) ;


  /*! \name Constructors and Destructors

    Private because the plugin manager should be a single static instance.
  */
  //@{
  /// Default constructor
  PluginManager() ;
  /// Copy constructor
  PluginManager(const PluginManager &pm) ;
  /// Destructor
  ~PluginManager() ;
  //@}

  /*! \brief Load a dynamic library

    The underlying worker method; this is where the actual business of loading
    is performed. Assumes that any necessary preprocessing of the path has been
    performed.
  */
  DynamicLibrary *loadLibrary(const std::string &path,
				  std::string & errorString) ;

  /*! \brief Executing plugin initialisation method?

    Whatever this is, it exists, but doesn't seem to be used, in Sayfan's
    original code.  -- lh, 110825 --
  */
  bool inInitializePlugin_ ;

  /// Map DynamicLibrary objects to names
  typedef std::map<std::string,DynamicLibrary*> DynamicLibraryMap ;
  /// Vector of plugin registration parameters
  typedef std::vector<RegisterParams> RegistrationVec ;
  /// Vector of plugin exit functions
  typedef std::vector<ExitFunc> ExitFuncVec ;
  /// Registration map
  typedef std::map<std::string,RegisterParams> RegistrationMap ;

  /// Get the plugin registration map
  const RegistrationMap &getRegistrationMap() ;


  PlatformServices platformServices_ ;
  DynamicLibraryMap   dynamicLibraryMap_ ;

  ExitFuncVec         exitFuncVec_ ;

  RegistrationMap     tempExactMatchMap_;   // register exact-match object types
  RegistrationVec     tempWildCardVec_;     // wild card ('*') object types

  RegistrationMap     exactMatchMap_;   // register exact-match object types
  RegistrationVec     wildCardVec_;     // wild card ('*') object types

  /// Default plugin directory
  std::string dfltPluginDir_ ;

  /// Message handler
  CoinMessageHandler *msgHandler_ ;
  /// Messages
  CoinMessages msgs_ ;

} ;

}  // end namespace Osi2
#endif
