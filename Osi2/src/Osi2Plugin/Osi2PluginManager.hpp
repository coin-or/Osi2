
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

  The act of loading a plugin library is an atomic transaction managed by
  #loadOneLib: The library must be found, loaded, and successfully execute
  its initialisation function before registration information is copied into
  the manager's client-accessible data structures.

  There is a single instance of the plugin manager (declared as a static
  object private to #getInstance).

  \todo What about unloading an individual library? Currently, all we can do
	is shut down the plugin manager, unloading all plugin libraries. For
	that matter, what about deregistering an API?
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

    Will invoke the exit method for all plugins, unload all libraries, and
    shut down the plugin manager.

    \todo It's minorly awkward that there's no way to associate a library with
	  an exit function. If the exit function fails, there's no way to say
	  which library it belongs to.
  */
  int shutdown() ;

  /// Get the default plugin directory
  inline std::string getDfltPluginDir() const { return (dfltPluginDir_) ; }

  /// Set the default plugin directory
  inline void setDfltPluginDir(std::string dfltDir)
  { dfltPluginDir_ = dfltDir ; }

  /*! \brief Load and initialise all plugin libraries in the directory.

    Currently unimplemented until we can create platform-independent file
    system support. For that matter, it's questionable whether we want this
    functionality.
  */
  int loadAllLibs(const std::string &pluginDirectory,
		  const InvokeServiceFunc func = NULL) ;

  /*! \brief Load and initialise the specified plugin library.

    Load the specified library \c dir/lib. If \p dir is not specified,
    #dfltPluginDir_ is used.

    \return
    - -3: initialisation function failed
    - -2: failed to find the initialisation function
    - -1: library failed to load
    -  0: library loaded and initialised without error
    -  1: library is already loaded

    \todo Should deal with things like symbolic links but that functionality
	  is currently disabled pending file system support.
  */
  int loadOneLib(const std::string &lib, const std::string *dir = 0) ;

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

  /*! \name Plugin manager control methods

    Miscellaneous methods that control the behaviour of the plugin manager.
  */
  //@{

  /// Set the log (verbosity) level
  inline void setLogLvl(int logLvl) { logLvl_ = logLvl ; }
  /// Get the log (verbosity) level
  inline int getLogLvl() const { return (logLvl_) ; }

  //@}

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

  /*! \brief Validate registration parameters

    Check the validity of a registration parameter block supplied by
    a plugin to register an API.
  */
  bool validateRegParams(const CharString *apiStr,
			 const RegisterParams *params) const ;

  /*! \brief Initialising a plugin?

    True during initialisation of a plugin library. Used to determine if plugin
    activity (API registration, etc) should go to temporary structures.
  */
  bool initialisingPlugin_ ;

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

  RegistrationMap     tmpExactMatchMap_;   // register exact-match object types
  RegistrationVec     tmpWildCardVec_;     // wild card ('*') object types

  RegistrationMap     exactMatchMap_;   // register exact-match object types
  RegistrationVec     wildCardVec_;     // wild card ('*') object types

  /// Default plugin directory
  std::string dfltPluginDir_ ;

  /// Message handler
  CoinMessageHandler *msgHandler_ ;
  /// Messages
  CoinMessages msgs_ ;
  /// Log (verbosity) level
  int logLvl_ ;

} ;

}  // end namespace Osi2
#endif
