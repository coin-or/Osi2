
/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/

#ifndef Osi2API_HPP
# define Osi2API_HPP

/// Hedge against C++0x
const int nullptr = 0 ;

namespace Osi2 {

/*! \brief Osi2 API virtual base class

  This class forms the root of the Osi2 API hierarchy. Its raison d'etre is to
  provide a common pointer for API objects returned from load and unload calls.

  The general philosophy of the API hierarchy is that the APIs are defined by
  abstract classes. Concrete implementations derive from the abstract classes.
*/

class API {

  public:

    /// Virtual destructor, should be overridden in any derived class
    virtual ~API () {}

} ;

} // end namespace Osi2

#endif
