/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/

#ifndef OsiControlAPI_Imp_HPP
# define OsiControlAPI_Imp_HPP

#include "Osi2PluginManager.hpp"

namespace Osi2 {

/*! \brief Osi2 ControlAPI implementation

  This class implements the Osi2 Control API (Osi2::ControlAPI).
*/
class ControlAPI_Imp : public ControlAPI {

public:

  /// \name Constructors and Destructors
  //@{
  /// Virtual constructor
  ControlAPI *create() ;
  /// Virtual copy constructor
  ControlAPI *clone() ;
  /// Destructor
  ~ControlAPI_Imp() ;
  /// Default constructor; \sa #create
  ControlAPI_Imp() ;
  /// Default copy constructor; \sa #clone
  ControlAPI_Imp(const ControlAPI_Imp &original) ;
  /// Assignment
  ControlAPI_Imp &operator=(const ControlAPI_Imp &rhs) ;
  //@}

  /*! \name API load and unload
      \brief Methods to load and unload objects implementing specific APIs.

      The general paradigm is that specific APIs are designated by names
      (strings), as are solvers.
  */
  //@{

  /*! \brief Load the specified API.

    A call to load a specific API may simply return a reference to an existing
    object implementing the API, or it may trigger the dynamic loading of a
    plugin which can supply an object implementing the specified API. A request
    for a specific solver is handled in the same manner: If the specified
    solver is already loaded, it will be used, otherwise it will be loaded.

    The load method returns the requested object as a generic API*; this can be
    cast to the requested API with dynamic_cast. This provides a semblance of
    type safety as the dynamic cast will fail if the client attempts to cast it
    to something other than what was requested.

    The method will return nullptr on failure; more detailed information may
    be available in \p rtncode. \p rtncode will be 0 if the call is
    successful.
  */
  API *load(std::string api, std::string solver, int &rtncode) ;

  /*! \brief Unload the specified API.

    Unloads the specified API. The return value will be 0 if all went smoothly,
    non-zero on error.
  */
  int unload(API *api) ;

  //@}

private:

  /// Utility method to find the plugin manager and cache a reference.
  PluginManager *findPluginMgr() ;
  /// Cached reference to plugin manager.
  PluginManager *pluginMgr_ ;

} ;

} // namespace Osi2 ;

#endif
