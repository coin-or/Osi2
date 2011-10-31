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
      : identInfo_(0)
    { } ;

    /// Virtual destructor.
    virtual ~API () { }

    /// Retrieve object identification information block
    inline const void *getIdentInfo () const { return (identInfo_) ; }

protected:

    /// Set the identification information
    inline void setIdentInfo(const void *identInfo)
    { identInfo_ = identInfo ; }

private:

    /*! \brief API object identification information

      This is a generic pointer which a control API implementation can
      use as it pleases. Implementors, note that the only parameter to
      ControlAPI::destroyObject is a pointer to the object to be destroyed.
      The information stored here must be sufficient to allow the control
      API implementation to determine how to destroy the object. Typically
      this will depend on the implementation of the control API and the
      underlying plugin manager.
    */
    const void *identInfo_ ;

} ;

} // end namespace Osi2

#endif
