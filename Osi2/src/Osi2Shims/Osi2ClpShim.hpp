
#ifndef Osi2ClpShim_H
#define Osi2ClpShim_H

/*
  $Id$
*/

#include "Osi2Plugin.hpp"
#include "Osi2DynamicLibrary.hpp"

/*!
  Prototype for an Osi2 solver shim for clp. It may gradually grow into a full
  shim, or it may be abandoned once we figure out how it ought to work.
*/

class Osi2ClpShim {

  public:

  /// Default constructor
  Osi2ClpShim() ;

  /*! \brief Object factory

    Create clp-specific objects to satisfy the Osi2 API specified as the
    \p objectType member of of \p params.
  */
  static void *create (Osi2_ObjectParams *params) ;

  /// Cleanup method
  static int32_t cleanup (void *param) ;

  /// Set libClp handle
  inline void setLibClp (Osi2DynamicLibrary *libClp) { libClp_ = libClp ; }
  /// Get libClp handle
  inline Osi2DynamicLibrary *getLibClp () const { return (libClp_) ; }
  /// Set verbosity
  inline void setVerbosity (int verbosity) { verbosity_ = verbosity ; }
  /// Get verbosity
  inline int getVerbosity () const { return (verbosity_) ; }

  private:

  /*! \brief Plugin manager services structure

    Structure passed in at plugin initialisation specifying services available
    from the plugin manager.
  */
  const Osi2_PlatformServices *services_ ;

  /// The handle for libClp
  Osi2DynamicLibrary *libClp_ ;

  /// Verbosity level for information messages
  int verbosity_ ;

} ;

/*! \brief Plugin initialisation method
    \relates Osi2ClpShim

  Given a parameter specifying the plugin manager's registration method,
  constructs a registration parameter object and calls the registration
  method.

  This method needs to have C linkage so it can be easily loaded with
  Osi2DynamicLibrary::getSymbol.
*/
extern "C"
Osi2_ExitFunc initPlugin (const Osi2_PlatformServices *services) ;

/*! \brief Plugin cleanup method
    \relates Osi2ClpShim

  This method handles any necessary cleanup prior to unloading the method. It
  will be passed to the plugin manager and invoked just before the plugin
  manager unloads the plugin.

  It's unclear to me that this method actually needs C linkage, since it's
  passed to the plugin manager as the return value of #initPlugin().
*/
extern "C"
int32_t cleanupPlugin () ;


#endif		// Osi2Clpshim_H

