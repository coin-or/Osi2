/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  Based on original design and code by Gigi Sayfan published in five parts
  in Dr. Dobbs, starting November 2007.
*/
/*! \file Osi2Plugin.hpp
    \brief Type definitions for the Osi2 plugin manager.

  This file contains the type definitions (types, structures, enums) used by
  the plugin framework. Generally useful for the plugin manager and for
  plugins.
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

/// A typedef for null-terminated strings, so they're easy to recognise
typedef uint8_t CharString ;

/// Languages (C, C++) supported by the plugin framework.
enum PluginLang
{ Plugin_C, Plugin_CPP } ;

/// Plugin manager API version number
struct PluginAPIVersion
{
  int32_t major_ ;
  int32_t minor_ ;
} ;

/*! \brief A plugin state object

  This object is responsible for keeping track of whatever information is
  necessary to manage the plugin's state with respect to objects supporting
  a given API.  A pointer to a plugin state object can be returned in the
  #RegisterParams object supplied by the client when it registers an API.
  It will be passed back to the plugin in the #ObjectParams structure when the
  plugin is asked to create or destroy an object of the given API.

  Typically, the referenced object is a instance of the class that implements
  the (static) \link Osi2::CreateFunc \endlink and \link Osi2::DestroyFunc
  \endlink methods. In essence, we're handing these static functions an
  explicit pointer to an instance of their class which can be used to
  maintain state information.
*/
typedef void PluginState ;

/*! \defgroup PluginFuncs Plugin Management Functions

  These methods are used to communicate between the plugin and the plugin
  manager. All methods must be defined by any plugin library, even if they
  do nothing.
*/
//@{
/*! \brief Constructor for a plugin object

  This function is implemented by the plugin and invoked by the plugin
  manager to create instances of objects (APIs) supported by the plugin. A
  \c CreateFunc must be specified for each registered API, but exactly
  how the function is implemented is a decision for the plugin author.

  \param parms \link Osi2::ObjectParams object parameters \endlink for use
  	   by the plugin.
  \returns Pointer to an object supporting the requested API, or null for
	   failure.
*/
typedef void *(*CreateFunc)(const ObjectParams *parms) ;

/*! \brief Destructor for a plugin object

  This function is implemented by the plugin and invoked by the plugin
  manager to destroy instances of objects (APIs) supported by the plugin. A
  \c DestroyFunc must be specified for each registered API, but exactly how
  the function is implemented is a decision for the plugin author.

  \param victim the object to be destroyed.
  \param parms \link Osi2::ObjectParams object parameters \endlink for use
  	   by the plugin.
  \returns 0 for success, nonzero for failure.
*/
typedef int32_t (*DestroyFunc)(void *victim, const ObjectParams *parms) ;

/*! \brief Function invoked by the plugin to register an API.

  This function is implemented by the plugin manager and passed to the plugin
  in a #PlatformServices parameter. The plugin should invoke this function to
  register each API that it can provide. Typically this method is called as
  part of the execution of the \link Osi2::Initfunc \endlink. It can also be
  invoked at any time while the plugin is active to register new APIs.

  \param apiStr A null-terminated character string identifying the API.
  \param parms  \link Osi2::RegisterParams registration information \endlink
		 for consumption by the plugin manager.
  \returns 0 for a successful registration, nonzero otherwise.
*/
typedef int32_t (*RegisterFunc)(const CharString *apiStr,
				const RegisterParams *parms) ;

/*! \brief Function to allow the plugin to invoke services provided by the
	   plugin manager.

  This method is implemented by the plugin manager and passed to the plugin in
  a #PlatformServices parameter object. The plugin can invoke this function to
  access services provided by the plugin manager.

  \param serviceName A null-terminated character string identifying the
		     service.
  \param serviceParams arbitrary parameter block appropriate to service

  \returns 0 if the service is successfully invoked, nonzero otherwise

  \todo Implement this! Currently unimplemented.
*/
typedef int32_t (*InvokeServiceFunc)(const CharString *serviceName,
					  void *serviceParams) ;

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
  #PluginManager to tell plugins to cleanup). If the initialization failed for
  any reason the plugin must return NULL. It may also (optionally) report the
  error via the error reporting function provided in the #PlatformServices
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
  /// Plugin manager version expected by the plugin
  PluginAPIVersion version_ ;
  /// Constructor for API being registered
  CreateFunc createFunc_ ;
  /// Destructor for API being registered
  DestroyFunc destroyFunc_ ;
  /// Implementation language of the API
  PluginLang lang_ ;
  /*! \brief Plugin control object for this API

    Optional pointer to an object holding state information used to manage
    objects supporting this API. This reference will be returned to the plugin
    as part of the \link #Osi2::ObjectParams parameters \endlink to the
    \link Osi2::CreateFunc create \endlink and \link Osi2::DestroyFunc destroy
    \endlink functions for objects of this API.
  */
  PluginState *ctrlObj_ ;
} ;

/*! \brief Parameters passed to a plugin for object creation and destruction.

  The functions to create and destroy an object are static methods, but for
  plugins capable of providing multiple APIs it may be useful to pass a
  pointer to the plugin object.
*/
struct ObjectParams
{
  /// Character string specifying the API(s) to be supplied
  const CharString *apiStr_ ;
  /*! \brief Services provided by the plugin manager

    See Osi2::PlatformServices.
  */
  const struct PlatformServices *platformServices_ ;
  /*! \brief Plugin control object for this API

    Optional pointer to an object holding state information used to manage
    objects supporting this API. This is the control object supplied with the
    \link Osi2::RegisterParams registration parameters \endlink for this API.
  */
  PluginState *ctrlObj_ ;
} ;

/*! \brief Attributes and methods provided by the plugin framework for use
	   by plugins.

  \todo Provide some services!
*/
struct PlatformServices
{
  /// Plugin manager version
  PluginAPIVersion version_ ;
  /// Default directory to search for plugins
  const CharString *dfltPluginDir_ ;
  /// Method to register an object supported by the plugin
  RegisterFunc registerObject_ ; 
  /*! \brief Method to allow the plugin to invoke a service provided by the
  	     plugin manager.
  */
  InvokeServiceFunc invokeService_; 
} ;


}  // end namespace Osi2

#ifdef  __cplusplus
}
#endif

#endif

