/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2API.hpp

  Defines the class Osi2::API, the top of the Osi2 API hierarchy.
*/

#ifndef Osi2API_HPP
# define Osi2API_HPP

/*! \brief Identification namespace for OSI2.

  All APIs and their implementations should be defined within the Osi2
  namespace. Also used to distinguish the implementation of Osi2.
*/
namespace Osi2 {

/*! \brief Osi2 API virtual base class

  The general philosophy of the API hierarchy is that the APIs are defined by
  abstract classes. Concrete implementations derive from the abstract classes.

  This class forms the root of the Osi2 API hierarchy. Its raison d'etre is to
  provide a common pointer for API objects returned from load and unload calls.
  It also provides a pointer where a control API can record information about
  the API object.
*/
class API {

public:

    /// Default constructor
    API ()
      : ctrlInfo_(0)
    { } ;

    /*! \brief Copy constructor
    
      C++ programmers will tend to use clone without thinking about it.
      Semantically, this is a problem because the clone was not created
      by Osi2ControlAPI::createObject.  When the user clones, the cloned
      object becomes their responsibility.  We need this to avoid passing
      the control information pointer on to the cloned object.
    */
    API (const API &orig)
      : ctrlInfo_(0)
    { } ;

    /// Virtual destructor.
    virtual ~API () { }

/*! \brief Object control information

  Set or retrieve object control information. See the comments for
  #ctrlInfo_.
*/
//@{
    /// Set the object control information block
    inline void setCtrlInfo(const void *ctrlInfo)
    { ctrlInfo_ = ctrlInfo ; }

    /// Retrieve the object control information block
    inline const void *getCtrlInfo () const { return (ctrlInfo_) ; }
//@}

private:

    /*! \brief API object control information

      This is an opaque pointer for exclusive use of the Osi2::ControlAPI.
      The exact content depends on the implementation of the control API and
      the underlying plugin manager.
    */
    const void *ctrlInfo_ ;

} ;

} // end namespace Osi2

#endif
