
#ifndef Osi2ClpShim_H
#define Osi2ClpShim_H

/*
  $Id$
*/

#include "Osi2Plugin.hpp"
#include "Osi2DynamicLibrary.hpp"

namespace Osi2 {
/*!
  Prototype for an Osi2 solver shim for clp. It may gradually grow into a full
  shim, or it may be abandoned once we figure out how it ought to work.
*/

class ClpShim {

  public:

  /// Default constructor
  ClpShim() ;

  /*! \brief Object factory

    Create clp-specific objects to satisfy the Osi2 API specified as the
    \p objectType member of of \p params.
  */
  static void *create (ObjectParams *params) ;

  /// Cleanup method
  static int32_t cleanup (void *param) ;

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

  /// Verbosity level for information messages
  int verbosity_ ;

} ;

/*! \brief Plugin initialisation method
    \relates ClpShim

  Given a parameter specifying the plugin manager's registration method,
  constructs a registration parameter object and calls the registration
  method.

  This method needs to have C linkage so it can be easily loaded with
  DynamicLibrary::getSymbol.
*/
extern "C"
ExitFunc initPlugin (const PlatformServices *services) ;

/*! \brief Plugin cleanup method
    \relates ClpShim

  This method handles any necessary cleanup prior to unloading the method. It
  will be passed to the plugin manager and invoked just before the plugin
  manager unloads the plugin.

  It's unclear to me that this method actually needs C linkage, since it's
  passed to the plugin manager as the return value of #initPlugin().
*/
extern "C"
int32_t cleanupPlugin () ;

}  // end namespace Osi2

#endif		// Clpshim_H

