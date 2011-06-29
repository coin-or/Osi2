
#ifndef OSI2PLUGIN_H
#define OSI2PLUGIN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Osi2_PluginLang
{ Osi2_Plugin_C, Osi2_Plugin_CPP } Osi2_PluginLang ;
 
struct Osi2_PlatformServices ;

typedef struct Osi2_ObjectParams
{
  const uint8_t *objectType ;
  const struct Osi2_PlatformServices *platformServices ;
  void *client ;
} Osi2_ObjectParams ;

typedef struct Osi2_PluginAPI_Version
{
  int32_t major ;
  int32_t minor ;
} Osi2_PluginAPI_Version ;

typedef void *(*Osi2_CreateFunc)(Osi2_ObjectParams *parms) ;
typedef int32_t (*Osi2_DestroyFunc)(void *parms) ;

/*! \brief The plugin client object

  The plugin client object is responsible for keeping track of whatever state
  is necessary to manage the underlying solver and factories that return
  objects to the plugin manager.

  As originally envisioned, this is an instance of the class that provides the
  (static) createFunc and destroyFunc. In essence, we're handing these static
  functions an explicit pointer to an instance of their class which they can
  use to keep state.
*/
typedef void* Osi2_PluginClient ;

typedef struct Osi2_RegisterParams
{
  Osi2_PluginAPI_Version version ;
  Osi2_CreateFunc createFunc ;
  Osi2_DestroyFunc destroyFunc ;
  Osi2_PluginLang lang ;
  Osi2_PluginClient client ;
} Osi2_RegisterParams ;


typedef int32_t (*Osi2_RegisterFunc)(const uint8_t *nodeType,
				     const Osi2_RegisterParams *params) ;
typedef int32_t (*Osi2_InvokeServiceFunc)(const uint8_t *serviceName,
					  void * serviceParams) ;

typedef struct Osi2_PlatformServices
{
  Osi2_PluginAPI_Version version ;
  Osi2_RegisterFunc registerObject; 
  Osi2_InvokeServiceFunc invokeService; 
} Osi2_PlatformServices ;


typedef int32_t (*Osi2_ExitFunc)() ;

/*! \brief Type definition of the #Osi2_initPlugin function below (used by
	   PluginManager to initialize plugins).
  
  Note the return type is the #Osi2_ExitFunc (used by PluginManager to tell
  plugins to cleanup). If the initialization failed for any reason the plugin
  may report the error via the error reporting function of the provided
  platform services. Nevertheless, it must return NULL exit func in this case
  to tell the plugin manager that the plugin wasn't initialized properly. The
  plugin may use the runtime services - allocate memory, log messages and of
  course register node types.
 
  \param  [const Osi2_PlatformServices *] params - the platform services struct 
  \retval [Osi2_ExitFunc] the exit func of the plugin or NULL if
			  initialization failed.
 */
typedef Osi2_ExitFunc (*Osi2_InitFunc)(const Osi2_PlatformServices *services) ;

/** 
 * Named exported entry point into the plugin
 * This definition is required eventhough the function 
 * is loaded from a dynamic library by name
 * and casted to Osi2_InitFunc. If this declaration is 
 * commented out DynamicLibrary::getSymbol() fails
 *
 * The plugin's initialization function MUST be called "Osi2_initPlugin"
 * (and conform to the signature of course).
 *
 * @param  [const Osi2_PlatformServices *] params - the platform services struct 
 * @retval [Osi2_ExitFunc] the exit func of the plugin or NULL if initialization failed.
 */

#ifndef PLUGIN_API
  #ifdef WIN32
    #define PLUGIN_API __declspec(dllimport)
  #else
    #define PLUGIN_API
  #endif
#endif

extern
#ifdef  __cplusplus
"C" 
#endif
PLUGIN_API Osi2_ExitFunc Osi2_initPlugin(const Osi2_PlatformServices * params) ;

#ifdef  __cplusplus
}
#endif

#endif /* OSI2PLUGIN_H */

