
/*
  Copyright 2011 Matt Saltzman, Lou Hafer
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/

#ifndef OsiControlAPI_HPP
# define OsiControlAPI_HPP

#include <string>

namespace Osi2 {

/*! \brief Osi2 ControlAPI virtual base class

  This abstract class defines the Osi2 control interface, including loading and
  unloading of objects.
*/
class ControlAPI : public API {

  public:

  /// \name Constructors and Destructors
  //@{
  /// Virtual constructor
  virtual ControlAPI *create() = 0 ;
  /// Virtual copy constructor
  virtual ControlAPI *clone() = 0 ;
  /// Virtual destructor
  virtual ~ControlAPI () = 0 ;
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
  virtual API *load(std::string api, std::string solver, int &rtncode) = 0 ;

  /*! \brief Unload the specified API.

    Unloads the specified API. The return value will be 0 if all went smoothly,
    non-zero on error.
  */
  virtual int unload(API *api) = 0 ;

} ;

} // namespace Osi2 ;

#endif
