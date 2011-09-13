/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  Based on original design and code by Gigi Sayfan published in five parts
  in Dr. Dobbs, starting November 2007.
*/
/*! \file Osi2Plugin.hpp
    \brief Type definitions for the Osi2 plugin manager.

  This file contains the type definitions (types, structures, enums) used by
  the plugin framework.
*/
#ifndef OSI2_PLUGIN_HPP
#define OSI2_PLUGIN_HPP

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace Osi2 {

// Forward declarations
struct PlatformServices ;
struct RegisterParams ;
struct ObjectParams ;

/// Languages (C, C++) supported by the plugin framework.
enum PluginLang
{ Plugin_C, Plugin_CPP } ;

/// Plugin manager API version number
struct PluginAPIVersion
{
  int32_t major ;
  int32_t minor ;
} ;

/*! \brief A plugin manager client (i.e., a plugin) object

  This object is responsible for keeping track of whatever information
  is necessary to manage the plugin's state (e.g., the underlying solver and
  factories that return objects to the plugin manager. A pointer to the
  client object is included in the #RegisterParams object returned by the
  client when it registers APIs.

  Typically, this is an instance of the class that implements the (static)
  \link Osi2::CreateFunc \endlink and \link Osi2::DestroyFunc \endlink
  methods. In essence, we're handing these static functions an explicit
  pointer to an instance of their class which can be used to maintain state
  information.
*/
typedef void* PluginMgrClient ;

/*! \defgroup PluginFuncs Plugin Management Functions

    These methods are used to communicate between the plugin and the plugin
    manager. All methods must be defined by any plugin library, even if they do
    nothing.
*/
//@{
/*! \brief Constructor for a plugin object

  This method is provided by the plugin and invoked to create instances of
  objects (APIs) supported by the plugin. A \c CreateFunc is required for each
  registered API, but a single CreateFunc can be used for multiple APIs. This
  is a decision for the plugin writer.

  \param parms \link Osi2::ObjectParams object creation parameters \endlink.
  \returns Pointer to an object supporting the requested API, or NULL for
	   failure.

  \todo Seems like this will need a pointer to the PluginMgrClient object
	(currently included in ObjectParams, but think about something more
	general that covers DestroyFunc, etc.).
*/
typedef void *(*CreateFunc)(ObjectParams *parms) ;

/*! \brief Destructor for a plugin object

  This method is provided by the plugin and invoked to destroy instances of
  objects (APIs) supplied by the plugin. As with the constructor, a \c
  DestroyFunc is required for each registered API, but how this is arranged is
  the business of the plugin writer.

  \param victim the object to be destroyed.
  \returns 0 for success, nonzero for failure.

  \todo Seems like this will need a pointer to the PluginMgrClient object.
*/
typedef int32_t (*DestroyFunc)(void *victim) ;

/*! \brief Function invoked by the plugin to register an API.

  This function is implemented by the plugin manager and passed to the plugin
  in a PlatformServices parameter. The plugin should invoke this function to
  register each API that it can provide. Typically this method is called as
  part of the execution of the \link Osi2::Initfunc \endlink. It can also be
  invoked at any time while the plugin is active to register new APIs.

  \param apiName A null-terminated character string identifying the API.
  \param params  \link Osi2::RegisterParams registration information \endlink
		 for consumption by the plugin manager.
  \returns 0 for a successful registration, nonzero otherwise.
*/
typedef int32_t (*RegisterFunc)(const uint8_t *apiName,
				const RegisterParams *params) ;

/*! \brief Function to allow the plugin to invoke services provided by the
	   plugin manager.

  This method is implemented by the plugin manager and passed to the plugin in
  a PlatformServices parameter. The plugin can invoke this function to access
  services provided by the plugin manager.

  \param serviceName A null-terminated character string identifying the
		     service.
  \param serviceParams arbitrary parameter block appropriate to service

  \returns 0 if the service is successfully invoked, nonzero otherwise

  \todo Implement this! Currently unimplemented.
*/
typedef int32_t (*InvokeServiceFunc)(const uint8_t *serviceName,
					  void * serviceParams) ;

/*! \brief Type definition of the plugin exit function

  This function is called by the plugin manager to tell the plugin to clean
  up in preparation for unloading the plugin library. A reference to this
  function must be returned as the return value of the
  \link Osi2::InitFunc \endlink.

  \returns 0 for successful execution, nonzero otherwise.
*/
typedef int32_t (*ExitFunc)() ;

/*! \brief Type definition of the \c initPlugin function

  The plugin manager will locate the \c initPlugin function with
  DynamicLibrary::getSymbol using the string \c "initPlugin" and call
  it to initialise the plugin library.

  Note the return type is \link Osi2::ExitFunc \endlink (used by
  PluginManager to tell plugins to cleanup). If the initialization failed for
  any reason the plugin must return NULL. It may also (optionally) report the
  error via the error reporting function provided in the PlatformServices
  parameter.

  \param services Services provided by the plugin manager.
  \returns The exit function of the plugin, or NULL if initialization fails.
 */
typedef ExitFunc (*InitFunc)(const PlatformServices *services) ;

//@}


/*! \brief Parameters required to register an API with the plugin manager

  The plugin supplies this structure when it registers with the plugin
  manager.
*/
struct RegisterParams
{
  /// Plugin manager API expected by the plugin
  PluginAPIVersion version ;
  /// Constructor for API being registered
  CreateFunc createFunc ;
  /// Destructor for API being registered
  DestroyFunc destroyFunc ;
  PluginLang lang ;
  PluginMgrClient client ;
} ;

/*! \brief Parameters passed to a plugin for object creation and destruction.

  The functions to create and destroy an object are static methods, but for
  plugins capable of providing multiple APIs it may be useful to pass a
  pointer to the plugin object.
*/
struct ObjectParams
{
  /// Character string specifying the API(s) to be supplied
  const uint8_t *objectType ;
  /*! \brief Services provided by the plugin manager

    See Osi2::PlatformServices.
  */
  const struct PlatformServices *platformServices ;
  /// Pointer to the plugin object
  PluginMgrClient client ;
} ;

/*! \brief Attributes and methods provided by the plugin framework for use
	   by plugins.
*/
struct PlatformServices
{
  /// Plugin manager version
  PluginAPIVersion version ;
  /// Default directory to search for plugins
  const uint8_t *dfltPluginDir ;
  /// Method to register an object supported by the plugin
  RegisterFunc registerObject; 
  /*! \brief Method to allow the plugin to invoke a service provided by the
  	     plugin manager.
  */
  InvokeServiceFunc invokeService; 
} ;


}  // end namespace Osi2

#ifdef  __cplusplus
}
#endif

#endif

