/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ClpShim.hpp
    
  Defines Osi2::ClpShim, a `lite' shim for clp that dynamically loads libClp.
*/

#ifndef Osi2ClpShim_HPP
#define Osi2ClpShim_HPP

#include "Osi2Plugin.hpp"
#include "Osi2DynamicLibrary.hpp"

namespace Osi2 {

/*! \brief `Lite' shim for the clp solver

  This shim is written to dynamically load libClp. As such, it doesn't need
  to be statically linked with libClp. The tradeoff is that it must work
  through clp's C interface. The shim, and  any objects it constructs, must
  dynamically load any methods they wish to use.
*/
class ClpShim {

public:

  /// Default constructor
  ClpShim() ;

  /*! \brief Object factory

    Create an object that supports the API(s) requested in \p params.

    \todo Decide how we might want to handle a request for an object that
    supports more than one %API.
  */
  static void *create (const ObjectParams *params) ;

  /*! \brief Object destructor

    Destroys an object created by this shim.
  */
  static int32_t destroy (void *victim, const ObjectParams *params) ;

  /// Set our unique ID (supplied by the plugin manager)
  inline void setPluginID (PluginUniqueID id) { ourID_ = id ; }
  /// Get our unique ID
  inline PluginUniqueID getPluginID () const { return (ourID_) ; }

  /// Set libClp handle
  inline void setLibClp (DynamicLibrary *libClp) { libClp_ = libClp ; }
  /// Get libClp handle
  inline DynamicLibrary *getLibClp () const { return (libClp_) ; }

  /// Set verbosity
  inline void setVerbosity (int verbosity) { verbosity_ = verbosity ; }
  /// Get verbosity
  inline int getVerbosity () const { return (verbosity_) ; }

private:

  /*! \brief Plugin manager services structure

    Structure passed in at plugin initialisation specifying services available
    from the plugin manager.
  */
  const PlatformServices *services_ ;

  /// The handle for libClp
  DynamicLibrary *libClp_ ;

  /// Our registration ID from the plugin manager
  PluginUniqueID ourID_ ;

  /// Verbosity level for information messages
  int verbosity_ ;

} ;

/*! \brief Plugin initialisation method
    \relates ClpShim

  Constructs a registration parameter object and calls the plugin manager's
  registration method.

  This method needs to have C linkage so it can be easily loaded with
  DynamicLibrary::getSymbol.
*/
extern "C"
ExitFunc initPlugin (PlatformServices *services) ;

/*! \brief Plugin cleanup method
    \relates ClpShim

  This method handles any necessary cleanup prior to unloading the plugin. It
  will be passed to the plugin manager and invoked just before the plugin
  manager unloads the plugin.
*/
extern "C"
int32_t cleanupPlugin (const PlatformServices *services) ;

}  // end namespace Osi2

#endif		// Clpshim_H

