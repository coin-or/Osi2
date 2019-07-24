/*
  Copyright 2019 Lou Hafer, Matt Saltzman, Kevin Poton
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ParamBEAPI.hpp

  The general notion is that the implementations underlying abstract APIs
  typically offer up a host of idiosyncratic parameters and interfaces
  to manipulate those parameters. ParamFEAPI and ParamBEAPI offer layers
  of syntactic sugar that reduce the effort required to manage parameters
  across multiple API objects.

  ParamBEAPI is the class-facing (back end) interface. The methods defined
  for this interface must be implemented by a class that wants to support
  parameter management.

  \sa ParamFEAPI
*/


#ifndef Osi2ParamBEAPI_HPP
#define Osi2ParamBEAPI_HPP

namespace Osi2 {

/*! \brief ParamBEAPI interface definitions

  This class defines the class-facing (back end) methods for the Osi2
  parameter management interface, a convenience API for handling parameters
  associated with plugin libraries.

  The methods defined in this class are of interest to Osi2 API implementors
  who want to provide parameter management capability for their API, and for
  implementors of an API providing user-facing parameter management.
*/
class ParamBEAPI {

public: 

  static const char *getAPIIDString () { return ("ParamBEAPI") ; }

/*! \name Backend parameter reporting and manipulation methods.

  These methods are implemented by an API that supports parameter management.
  They provide the capability for the API to report the parameters that it
  exports and set and get methods for those parameters.

  As explained above for ParamFEAPI, it's the responsibility of the API
  implementor to document the parameters available and the contents of blobs
  used to set and get values.
*/
//@{

  /*! \brief Return an array of parameter identifiers available for management.

    Each array entry should be a null-terminated string.
  */
  virtual int reportParams(const char **&params) = 0 ;

  /// Report the value of the requested parameter
  virtual bool get(const char *ident, void *&blob) = 0 ;

  /// Set the value of the requested parameter
  virtual bool set(const char *ident, const void *&blob) = 0 ;
//@}

} ;

} // namespace Osi2

#endif //  Osi2ParamBEAPI_HPP
