/*
  Copyright 2019 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ParamMgmtAPI.hpp

  Defines ParamMgmtAPI, an %API to provide some
  uniformity and syntactic sugar for setting and getting parameter values.
*/


#ifndef Osi2ParamMgmtAPI_HPP
#define Osi2ParamMgmtAPI_HPP

#include "Osi2ParamFEAPI.hpp"
#include "Osi2ParamBEAPI.hpp"

namespace Osi2 {

/*! \brief OSI2 Parameter Management %API

  This class defines the interface for the OSI2 parameter management %API.
  It's a trivial extension of the basic interface defined in ParamFEAPI,
  adding virtual constructors, a destructor, and log control.

  \todo
  This class is really unnecessary. Its main purpose in life is to provide a
  more user-friendly name than ParamFEAPI. Think about how best to eliminate
  it.
*/
class ParamMgmtAPI : public ParamFEAPI {

public:

  /*! \name Constructors and Destructors */
  //@{
    /// Virtual constructor
    virtual ParamMgmtAPI *create() = 0 ;
    /// Virtual copy constructor
    virtual ParamMgmtAPI *clone() = 0 ;
    /// Destructor
    virtual ~ParamMgmtAPI() { }
  //@}

  static const char *getAPIIDString () { return ("Osi2::ParamMgmtAPI") ; }

/*! \name ParamMgmt API control methods

  Miscellaneous methods that control the behaviour of a ParamMgmtAPI object.
*/
//@{

  /// Set the log (verbosity) level
  virtual void setLogLvl(int logLvl) = 0 ;

  /// Get the log (verbosity) level
  virtual int getLogLvl() const = 0 ;

//@}

} ;

} // namespace Osi2 ;

#endif
