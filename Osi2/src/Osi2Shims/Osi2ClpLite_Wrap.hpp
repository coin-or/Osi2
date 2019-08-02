/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ClpLite_Wrap.hpp
    
  Defines Osi2::ClpLite_Wrap, a wrapper class to contain objects created by
  Osi2::ClpShim (the `lite' shim).

  A bit of an experiment in %API support through composition. --lh, 190730 --
*/

#ifndef Osi2ClpLite_Wrap_HPP
#define Osi2ClpLite_Wrap_HPP

#include "Osi2API.hpp"
#include "Osi2APIMgmt_Imp.hpp"
#include "Osi2ClpSimplexAPI_ClpLite.hpp"

namespace Osi2 {

class ClpSimplexAPI ;

/*! \brief Clp `lite' wrapper class

  This is a wrapper class for objects constructed by ClpShim (the `lite'
  shim). It is intended as a container for objects that implement individual
  APIs.
*/
class ClpLite_Wrap : public API {

public:

/*! \name Constructors & related */
//@{
  /// Default constructor
  ClpLite_Wrap ()
    : clp_(nullptr)
  { }
  /// Destructor
  ~ClpLite_Wrap () { delete clp_  ; }
//@}

/*! \name API reporting */
//@{
  /// Return supported APIs
  inline int getAPIs (const char **&idents)
  { return (apiMgr_.getAPIs(idents)) ; }
  /// Return object supporting the specified %API
  inline void *getAPIPtr (const char *ident)
  { return (apiMgr_.getAPIPtr(ident)) ; }
//@}

/*! \name Methods to add support for an %API */
//@{
  /// Add a ClpSimplex object
  void addClpSimplex(DynamicLibrary *libClp) ;
//@}

private:

  /// API managment object
  APIMgmt_Imp apiMgr_ ;

  /// Object to implement the ClpSimplex interface
  ClpSimplexAPI_ClpLite *clp_ ;

} ;


}    // namespace Osi2

#endif    // Osi2ClpLite_Wrap_HPP
