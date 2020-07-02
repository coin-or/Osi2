/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/

/*! \file Osi2nullptr.hpp

  Declares a nullptr object and some convenience methods; intended as a hedge
  against the introduction of nullptr in C++0x.
*/

#ifndef Osi2NullPtr_HPP
# define Osi2NullPtr_HPP

#ifndef COIN_HAS_NULLPTR

/*! \brief Fake nullptr as a hedge against the transition to C++0x

  Cribbed from Sutter & Stroustrop, SC22/WG21/N2431, A name for the null
  pointer: nullptr (revision 4). The authors reference S. Meyers, More
  Effective C++, 2e, Addison Wesley, 1996, Item 25.
*/
class nullptr_t {

public:
  /// Conversion operator to any type of non-member pointer
  template<class T> operator T* () const { return (0) ; }
  /// Conversion operator to any type of member pointer
  template<class C, class T> operator T C::* () const { return (0) ; }
private:
  /// Cannot take the address of this object.
  void operator& () const ;

} ;

/*! \brief The actual object for use in assignment and comparison.
    \relates nullptr_t
*/
const nullptr_t nullptr = {} ;

/*! \name Equality operators
  
  So that we can write \code if (ptr == nullptr) \endcode and similar.
*/
//@{
/*! \brief Equality
    \relates nullptr_t
*/
template<class T> bool operator== (T op1, nullptr_t op2)
{ return (op1 == 0) ; }

/*! \brief Equality
    \relates nullptr_t
*/
template<class T> bool operator== (nullptr_t op1, T op2)
{ return (op2 == 0) ; }

/*! \brief Inequality
    \relates nullptr_t
*/
template<class T> bool operator!= (T op1, nullptr_t op2)
{ return (op1 != 0) ; }

/*! \brief Inequality
    \relates nullptr_t
*/
template<class T> bool operator!= (nullptr_t op1, T op2)
{ return (op2 != 0) ; }
//@}

#endif    // !COIN_HAS_NULLPTR

#endif
