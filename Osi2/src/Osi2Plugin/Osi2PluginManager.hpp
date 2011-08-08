
#ifndef OSI2PLUGINMANAGER_H
#define OSI2PLUGINMANAGER_H

#include <vector>
#include <map>
#include "Osi2Plugin.hpp"


namespace Osi2 {

class DynamicLibrary ;
struct IObjectAdapter ;

class PluginManager
{

  typedef std::map<std::string,DynamicLibrary*> DynamicLibraryMap ;
  typedef std::vector<ExitFunc> ExitFuncVec ;
  typedef std::vector<RegisterParams> RegistrationVec ;

  public:

  typedef std::map<std::string,RegisterParams> RegistrationMap ;

  /*! \brief Get a pointer to the plugin manager

    Returns a pointer to the single instance of the plugin manager.
  */
  static PluginManager &getInstance() ;

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

  /*! \brief Initialise a plugin

    Invokes the plugin's initialisation method
  */
  static int32_t initializePlugin(InitFunc initFunc) ;

  /*! \brief Register an object type with the plugin manager

    Invoked by plugins to register the objects they can create.
  */
  static int32_t registerObject(const uint8_t *nodeType,
                                const RegisterParams *params) ;

  /// Get the plugin registration map
  const RegistrationMap &getRegistrationMap() ;

  /// Get the services provided by the plugin manager
  PlatformServices &getPlatformServices() ;


  /*! \brief Invoked by application to create an object

    Plugins register the objects they can create. This method is invoked by the
    application to request an object. The pointer returned must be cast to
    the appropriate type.

    An explanation of adapter is needed somewhere.
  */
  void *createObject(const std::string &objectType, IObjectAdapter &adapter) ;

private:

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
private:
  bool                inInitializePlugin_ ;
  PlatformServices platformServices_ ;
  DynamicLibraryMap   dynamicLibraryMap_ ;
  ExitFuncVec         exitFuncVec_ ;

  RegistrationMap     tempExactMatchMap_;   // register exact-match object types
  RegistrationVec     tempWildCardVec_;     // wild card ('*') object types

  RegistrationMap     exactMatchMap_;   // register exact-match object types
  RegistrationVec     wildCardVec_;     // wild card ('*') object types

  /// Default plugin directory
  std::string dfltPluginDir_ ;

} ;

}  // end namespace Osi2
#endif
