
/*
  Method definitions for Osi2ClpShim.

  $Id$
*/

#include <iostream>

#include "Osi2ClpShim.hpp"
#include "Clp_C_Interface.h"
#include "ClpConfig.h"

#include "Osi2DynamicLibrary.hpp"


/*
  Default constructor.
*/
Osi2ClpShim::Osi2ClpShim ()
  : services_(0),
    verbosity_(1),
    libClp_(0)
{ }

typedef Clp_Simplex *(*ClpFactory)() ;
typedef void *(*ClpSimplexFactory)(Clp_Simplex *clp) ;

/*! \brief Object factory

  Create clp-specific objects to satisfy the Osi2 API specified as the
  \p objectType member of of \p params.
*/
void *Osi2ClpShim::create (Osi2_ObjectParams *params)
{
  std::string what = reinterpret_cast<const char *>(params->objectType) ;
  void *retval = 0 ;

  std::cout << "Clp create: type " << what << "." << std::endl ;

  if (what == "ClpSimplex") {
    std::cout
      << "Request to create ClpSimplex recognised." << std::endl ;
    Osi2ClpShim *shim = static_cast<Osi2ClpShim*>(params->client) ;
    Osi2DynamicLibrary *libClp = shim->libClp_ ;
    ClpFactory factory =
      reinterpret_cast<ClpFactory>(libClp->getSymbol("Clp_newModel")) ;
    if (factory == 0) {
      std::cout
	<< "Apparent failure to find Clp_newModel." << std::endl ;
      return (0) ;
    }
    Clp_Simplex *wrapper = factory() ;
    ClpSimplexFactory underlyingModel =
      reinterpret_cast<ClpSimplexFactory>(libClp->getSymbol("Clp_model")) ;
    if (underlyingModel == 0) {
      std::cout
	<< "Apparent failure to find Clp_model." << std::endl ;
      return (0) ;
    }
    retval = underlyingModel(wrapper) ;
  } else {
      std::cout
	<< "Clp create: unrecognised type " << what << "." << std::endl ;
  }

  return (retval) ;
}

/*! \brief Cleanup method

  Hmmm ... how to keep track when we have no apparent object?
*/
int32_t Osi2ClpShim::cleanup (void *params)
{
  return (0) ;
}


/*
  Plugin initialisation method. The job here is to construct a parameter
  structure which can be passed to the plugin manager's registration method
  (supplied in the services parameter). The return value is the exit method to
  be called before the plugin is unloaded.
*/
extern "C"
Osi2_ExitFunc initPlugin (const Osi2_PlatformServices *services)
{
  std::string version = CLP_VERSION ;
  std::cout
    << "Executing Osi2ClpShim::initPlugin, clp version "
    << version << "." << std::endl ;
/*
  Attempt to load clp.
*/
  std::string libClpName = "libClp.so.0" ;
  std::string libPath = "/cs/mitacs4/Osi2/Coin-Osi2-SunX86/lib/" ;
  std::string errMsg ;
  std::string fullPath = libPath+libClpName ;
  Osi2DynamicLibrary *libClp = Osi2DynamicLibrary::load(fullPath,errMsg) ;
  if (libClp == 0) {
    std::cout
      << "Apparent failure opening " << fullPath << "." << std::endl ;
    std::cout
      << "Error is " << errMsg << "." << std::endl ;
    return (0) ;
  }
/*
  Arrange to remember the handle to libClp, and in general remember what we're
  doing with an Osi2ClpShim object. Stash the handle for libClp in the
  Osi2ClpShim, then stash a pointer to the shim in Osi2_RegisterParams. This
  allows the plugin manager to hand back the shim object with each call, which
  in turn allows us to remember what we're doing.
*/
  Osi2_RegisterParams reginfo ;
  Osi2ClpShim *shim = new Osi2ClpShim() ;
  shim->setLibClp(libClp) ;
  reginfo.client = static_cast<void*>(shim) ;
/*
  Fill in the rest of the registration parameters and invoke the registration
  method.
*/
  reginfo.version.major = 1 ;
  reginfo.version.minor = CLP_VERSION_MINOR ;
  reginfo.lang = Osi2_Plugin_CPP ;
  reginfo.createFunc = Osi2ClpShim::create ;
  reginfo.destroyFunc = Osi2ClpShim::cleanup ;
  int retval =
      services->registerObject(reinterpret_cast<const unsigned char*>("ClpSimplex"),
			       &reginfo) ;
  if (retval < 0) {
    std::cout
      << "Apparent failure to register ClpSimplex plugin." << std::endl ;
    return (0) ;
  }

  return (cleanupPlugin) ;
}

/*
  Plugin cleanup method. Does whatever is needed to clean up after the plugin
  prior to unloading the library.
*/
extern "C" int32_t cleanupPlugin ()
{
  return (0) ;
}

