/*
  Copyright 2019 Lou Hafer, Matt Saltzman, Kevin Poton
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ParamFEAPI.hpp

  This file defines the user-facing (front end) interface for the OSI2
  parameter management %API, a convenience %API for handling parameters
  associated with %API objects.
*/


#ifndef Osi2ParamFEAPI_HPP
#define Osi2ParamFEAPI_HPP

#include <string>

namespace Osi2 {

/*! \brief OSI2 Parameter management front-end interface

  The general notion is that the implementations underlying abstract
  APIs typically offer up a host of idiosyncratic parameters and equally
  idiosyncratic interfaces to manipulate those parameters. ParamFEAPI offers
  a layer of syntactic sugar that reduces the effort required to manage
  parameters across the multiple %API objects at play in an application.

  ParamFEAPI is the user-facing (front end) interface, meant to be used from
  user code to register an API object for parameter management and get and
  set parameters. ParamBEAPI is the class-facing (back end) interface, meant
  to be used by implementors of an %API.
*/
class ParamFEAPI {

public:

  /// Return the "ident" string for the parameter management front-end %API.
  inline static const char *getAPIIDString () { return ("ParamFEAPI") ; }

/*! \name Parameter enrollment and manipulation methods

  These methods enroll an object (the `enrollee') with the parameter
  manager or remove it, and provide the ability to get and set parameter
  values registered with the manager by the enrollee.

  The user assigns an identity string to each enrollee.
  This identity string allows the parameter manager to locate
  the set of parameters for that object and direct get and set requests
  to the correct object.  In particular, this allows for multiple objects
  of the same type to be under management.

  The blob supplied by the user program for #get and #set will be handed
  verbatim to the enrollee. Similarly, the answer returned by the enrollee
  in response to a request for a parameter value is passed verbatim to
  the user. Because parameters are idiosyncratic, it's the responsibility
  of the API implementor to document the ident string and associated blob
  for a parameter and the responsibility of the user program to construct
  a suitable blob and interpret a returned blob.

  This isn't nearly as complicated as it sounds for many common parameters.
  Assuming an object, \p manager, that implements ParamFEAPI, and an object
  \p obj that supports parameter management, all that's needed to get or
  set the value of a simple integer parameter "Parm1" is
  \code{.cpp}
    manager.enroll("Enrollee",obj) ;

    int blob ;
    manager.get("Enrollee","Parm1",&blob) ;

    blob = 42 ;
    manager.set("Enrollee","Parm1",&blob) ;
  \endcode
  If the parameter requires something more than a simple data type, it's the
  user's responsibility to construct the appropriate blob.
*/
//@{

  /// Enroll an object and its parameters with the manager.
  virtual bool enroll(std::string ident, Osi2::API *object) = 0 ;

  /// Remove an object and its parameters from the manager.
  virtual bool remove(std::string ident) = 0 ;

  /// Get a parameter
  virtual bool get(std::string ident, std::string param, void *blob) = 0 ;

  /// Set a parameter
  virtual bool set(std::string ident, std::string param, const void *blob) = 0 ;
//@}

} ;

}   // namespace Osi2


#endif //  Osi2ParamFEAPI_HPP
