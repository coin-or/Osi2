/*
  Copyright 2019 Lou Hafer, Matt Saltzman, Kevin Poton
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ParamMgmtAPI.hpp

  Defines the virtual class Osi2::ParamMgmtAPI, an API to manage the
  parameters registered by libraries and objects.

  The general notion here is that underlying libraries (e.g., solvers) offer
  up a host of idiosyncratic parameters and interfaces to manipulate those
  parameters. The ParamMgmt API offers a layer of syntactic sugar that reduces
  the effort required to adjust parameters.
*/


#ifndef Osi2ParamMgmtAPI_HPP
# define Osi2ParamMgmtAPI_HPP

#include <string>

#include "Osi2API.hpp"

namespace Osi2 {

/*! \brief Osi2 ParamMgmtAPI virtual base class

  This abstract class defines the Osi2 control interface, including loading and
  unloading of objects.
*/
class ParamMgmtAPI : public API {

public:

    /// \name Constructors and Destructors
    //@{
    /// Virtual constructor
    virtual ParamMgmtAPI *create() = 0 ;
    /// Virtual copy constructor
    virtual ParamMgmtAPI *clone() = 0 ;
    /// Virtual destructor
    virtual ~ParamMgmtAPI() {}
    //@}

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
