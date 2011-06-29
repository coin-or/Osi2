
#ifndef OSI2PLUGINMANAGER_H
#define OSI2PLUGINMANAGER_H

#include <vector>
#include <map>
#include "Osi2Plugin.hpp"

class Osi2DynamicLibrary ;

struct IObjectAdapter ;

class Osi2PluginManager
{

  typedef std::map<std::string,Osi2DynamicLibrary*> DynamicLibraryMap ;
  typedef std::vector<Osi2_ExitFunc> ExitFuncVec ;
  typedef std::vector<Osi2_RegisterParams> RegistrationVec ;

  public:

  typedef std::map<std::string,Osi2_RegisterParams> RegistrationMap ;

  /*! \brief Get a pointer to the plugin manager

    Returns a pointer to the single instance of the plugin manager.
  */
  static Osi2PluginManager &getInstance() ;

  /*! \brief Shut down the plugin manager

    Will invoke the exit method for all plugins, then shut down the plugin
    manager.
  */
  int32_t shutdown() ;

  /*! \brief Scan a directory and load all plugins

    Currently unimplemented until we can create platform-independent file
    system support.
  */
  int32_t loadAll(const std::string &pluginDirectory,
		  Osi2_InvokeServiceFunc func = NULL) ;

  /*! \brief Load the specified plugin

    Should deal with things like symbolic links but that function is
    currently disabled pending file system support.
  */
  int32_t loadByPath(const std::string & path) ;

  /*! \brief Initialise a plugin

    Invokes the plugin's initialisation method
  */
  static int32_t initializePlugin(Osi2_InitFunc initFunc) ;

  /*! \brief Register an object type with the plugin manager

    Invoked by plugins to register the objects they can create.
  */
  static int32_t registerObject(const uint8_t *nodeType,
                                const Osi2_RegisterParams *params) ;

  /// Get the plugin registration map
  const RegistrationMap &getRegistrationMap() ;

  /// Get the services provided by the plugin manager
  Osi2_PlatformServices &getPlatformServices() ;


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
  Osi2PluginManager() ;
  /// Copy constructor
  Osi2PluginManager(const Osi2PluginManager &pm) ;
  /// Destructor
  ~Osi2PluginManager() ;
  //@}

  /*! \brief Load a dynamic library

    The underlying worker method; this is where the actual business of loading
    is performed. Assumes that any necessary preprocessing of the path has been
    performed.
  */
  Osi2DynamicLibrary *loadLibrary(const std::string &path,
				  std::string & errorString) ;
private:
  bool                inInitializePlugin_ ;
  Osi2_PlatformServices platformServices_ ;
  DynamicLibraryMap   dynamicLibraryMap_ ;
  ExitFuncVec         exitFuncVec_ ;

  RegistrationMap     tempExactMatchMap_;   // register exact-match object types
  RegistrationVec     tempWildCardVec_;     // wild card ('*') object types

  RegistrationMap     exactMatchMap_;   // register exact-match object types
  RegistrationVec     wildCardVec_;     // wild card ('*') object types
} ;

#endif
