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
#ifndef Osi2Plugin_HPP
#define Osi2Plugin_HPP

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace Osi2 {

// Forward declarations
struct PlatformServices ;

/// Languages (C, C++) supported by the plugin framework.
typedef enum PluginLang
{ Plugin_C, Plugin_CPP } PluginLang ;

/*! \brief A plugin client object

  The plugin client object is responsible for keeping track of whatever state
  is necessary to manage the underlying solver and factories that return
  objects to the plugin manager.

  As originally envisioned, this is an instance of the class that provides the
  (static) createFunc and destroyFunc. In essence, we're handing these static
  functions an explicit pointer to an instance of their class which they can
  use to keep state.
*/
typedef void* PluginClient ;

/*! \brief Parameters passed to a plugin for object creation and destruction.

  The functions to create and destroy an object are static methods, but for
  plugins capable of providing multiple APIs it may be useful to pass a
  pointer to the plugin object.
*/
typedef struct ObjectParams
{
  /// Character string specifying the API(s) to be supplied
  const uint8_t *objectType ;
  /*! \brief Services provided by the plugin manager

    See Osi2::PlatformServices.
  */
  const struct PlatformServices *platformServices ;
  /// Pointer to the plugin object
  PluginClient client ;
} ObjectParams ;

/// Plugin manager API version number
typedef struct PluginAPIVersion
{
  int32_t major ;
  int32_t minor ;
} PluginAPIVersion ;

/*! \brief Constructor for a plugin object

  This method is provided by the plugin and invoked to create instances of
  objects (APIs) supported by the plugin.
*/
typedef void *(*CreateFunc)(ObjectParams *parms) ;
/*! \brief Destructor for a plugin object

  This method is provided by the plugin and invoked to destroy instances of
  objects (APIs) supplied by the plugin.
*/
typedef int32_t (*DestroyFunc)(void *parms) ;

/*! \brief Parameters required to register an API with the plugin manager

  The plugin supplies this structure when it registers with the plugin
  manager.
*/
typedef struct RegisterParams
{
  /// Plugin manager API expected by the plugin
  PluginAPIVersion version ;
  /// Constructor for API being registered
  CreateFunc createFunc ;
  /// Destructor for API being registered
  DestroyFunc destroyFunc ;
  PluginLang lang ;
  PluginClient client ;
} RegisterParams ;

/*! \brief Function invoked by the plugin to register an API type.

  The plugin should invoke this function for each API that it can provide.
*/
typedef int32_t (*RegisterFunc)(const uint8_t *nodeType,
				     const RegisterParams *params) ;
/*! \brief Function to allow the plugin to invoke services provided by the
	   plugin manager.
*/
typedef int32_t (*InvokeServiceFunc)(const uint8_t *serviceName,
					  void * serviceParams) ;

/*! \brief Attributes and methods provided by the plugin framework for use
	   by plugins.
*/
typedef struct PlatformServices
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
} PlatformServices ;

/*! \brief Type definition of the plugin exit function

  Called by the plugin manager to tell the plugin to clean up in preparation
  for unloading by the plugin manager.
*/
typedef int32_t (*ExitFunc)() ;

/*! \brief Type definition of the #initPlugin function below (used by
	   PluginManager to initialize plugins).
  
  Note the return type is the #ExitFunc (used by PluginManager to tell
  plugins to cleanup). If the initialization failed for any reason the plugin
  may report the error via the error reporting function of the provided
  platform services. Nevertheless, it must return NULL exit func in this case
  to tell the plugin manager that the plugin wasn't initialized properly. The
  plugin may use the runtime services - allocate memory, log messages and of
  course register node types.
 
  \param services The platform services structure 
  \returns The exit function of the plugin, or NULL if initialization fails.
 */
typedef ExitFunc (*InitFunc)(const PlatformServices *services) ;

/*! \brief Function to initialise the plugin 
 
  Should do whatever initialisation is required once the plugin is loaded.

  \param  params - the platform services struct 
  \returns The exit function of the plugin or NULL if initialization fails.
 */
ExitFunc initPlugin(const PlatformServices *params) ;

}  // end namespace Osi2

#ifdef  __cplusplus
}
#endif

#endif

