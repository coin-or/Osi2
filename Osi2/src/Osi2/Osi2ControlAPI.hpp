
/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/

#ifndef Osi2ControlAPI_HPP
# define Osi2ControlAPI_HPP

#include <string>

#include "Osi2API.hpp"

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
  virtual ~ControlAPI() {}
  //@}

  /*! \name Library load and unload
      \brief Methods to load and unload plugin libraries

    Given only a short name xxx the method will look for libOsi2XxxShim.so in
    the default plugin library directory. The other load methods allow for an
    arbitrary mapping between the short name and the plugin library.
  */
  //@{

  /*! \brief Load the specified plugin library

    Given \p shortName 'xxx', attempt to load libOsi2XxxShim.so from the
    default plugin library directory. Associate the plugin library with
    \p shortName.
  */
  virtual int load(const std::string &shortName) = 0 ;

  /*! \brief Load the specified plugin library

    Attempt to load library \p libName from the default plugin library
    directory. Associate the plugin library with \p shortName.
  */
  virtual int load(const std::string &shortName,
  		   const std::string &libName) = 0 ;

  /*! \brief Load the specified plugin library

    Attempt to load library \p libName from directory \p dirName. Associate
    the plugin library with \p shortName.
  */
  virtual int load(const std::string &shortName,
  		   const std::string &libName, const std::string *dirName) = 0 ;

  /*! \brief Unload the specified library.

    Unloads the specified library. The return value will be 0 if all went
    smoothly, non-zero on error.
  */
  virtual int unload(const std::string &shortName) = 0 ;

  //@}

  /*! \brief Create an object of the specified API

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

  virtual API *create(std::string api, std::string solver, int &rtncode) = 0 ;
  */


} ;

} // namespace Osi2 ;

#endif
