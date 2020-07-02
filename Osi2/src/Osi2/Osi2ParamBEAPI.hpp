/*
  Copyright 2019 Lou Hafer, Matt Saltzman, Kevin Poton
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ParamBEAPI.hpp

  This file defines the class-facing (back end) interface for the OSI2
  parameter management %API, a convenience %API for handling paramters
  associated with %API objects.
*/


#ifndef Osi2ParamBEAPI_HPP
#define Osi2ParamBEAPI_HPP

namespace Osi2 {

/*! \brief OSI2 parameter management back-end interface.

  The general notion is that the implementations underlying abstract
  APIs typically offer up a host of idiosyncratic parameters and equally
  idiosyncratic interfaces to manipulate those parameters.  The user-facing
  interface, ParamFEAPI, offers a layer of syntactic sugar that reduces the
  effort required to manage parameters across the multiple %API objects at
  play in an application.

  This class defines the class-facing (back end) methods for the OSI2
  parameter management interface.  The methods defined for this interface
  must be provided by the implementation of an %API that wants to support
  parameter management.

  The methods defined in this class are of interest to OSI2 %API implementors
  who want to provide parameter management capability for their %API
  implementation, and for implementors of an %API providing user-facing
  parameter management.
*/
class ParamBEAPI {

public: 

  /// Return the "ident" string for the parameter management back-end %API.
  inline static const char *getAPIIDString () { return ("ParamBEAPI") ; }

/*! \name Backend parameter reporting and manipulation methods.

  These methods are implemented by an API that supports parameter management.
  They provide the capability for the API to report the parameters that it
  exports and to get and set the values of those parameters.

  It's the responsibility of the API implementor to document the identifying
  strings for the exported parameters and the contents of blobs used to set
  available and get values.
*/
//@{

  /*! \brief Return an array of parameter identifiers available for
  	     management.

    Each array entry should be a null-terminated string.
  */
  virtual int reportParams(const char **&params) = 0 ;

  /*! Get the value of the requested parameter

    The value should be returned in the provided \p blob. A return value of
    false means failure to retrieve the value.
  */
  virtual bool get(const char *ident, void *&blob) = 0 ;

  /*! Set the value of the requested parameter

    Set the parameter to the value provided in \p blob. A return value of
    false means the parameter was not set.
  */
  virtual bool set(const char *ident, const void *&blob) = 0 ;
//@}

} ;

} // namespace Osi2

#endif //  Osi2ParamBEAPI_HPP
