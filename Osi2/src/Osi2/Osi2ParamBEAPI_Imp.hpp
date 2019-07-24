/*
  Copyright 2019 Lou Hafer, Matt Saltzman, Kevin Poton
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ParamBEAPI_Imp.hpp

  This file defines a class that implements the ParamBEAPI interface. It is
  intended to be used as a member object within an API class implementation to
  provide a parameter management interface.
*/


#ifndef Osi2ParamBEAPI_Imp_HPP
#define Osi2ParamBEAPI_Imp_HPP

#include <map>

#include "Osi2API.hpp"

#include "Osi2ParamBEAPI.hpp"
#include "Osi2ParamBEAPIMessages.hpp"

namespace Osi2 {

/*! \brief An implementation of the parameter management back end.

  This class implements the ParamBEAPI methods that must be supplied by an API
  if it supports parameter management. It is intended to be used by including
  an Osi2ParamBEAPI_Imp object as a member in the API object that is to be
  managed.
*/
template<class Managed>
class ParamBEAPI_Imp : public ParamBEAPI {

public: 

  static const char *getAPIIDString ()
  { return (Osi2::ParamBEAPI::getAPIIDString()) ; }

/// \name Constructors and Destructor
    //@{
    /// Default constructor; \sa #create
    ParamBEAPI_Imp() ;
    /// Default copy constructor; \sa #clone
    ParamBEAPI_Imp(const ParamBEAPI_Imp &original) ;
    /// Virtual constructor
    ParamBEAPI *create() ;
    /// Virtual copy constructor
    ParamBEAPI *clone() ;
    /// Destructor
    ~ParamBEAPI_Imp() ;
    /// Assignment
    ParamBEAPI_Imp &operator=(const ParamBEAPI_Imp &rhs) ;
    //@}

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
  inline int reportParams (const char **&params)
  { params = paramIDs_ ;  return (paramIDCnt_) ; }

  /// Report the value of the requested parameter
  bool get(const char *ident, void *&blob) ;

  /// Set the value of the requested parameter
  bool set(const char *ident, const void *&blob) ;
//@}

/*! \name Utilities for use by the managed object

  Utility functions to allow the managed object to load information into this
  object and retrieve it.
*/
  /// Add a capability API
  void addAPIID (const char *apiID, void *obj) ;
  /// Retrieve the list of capability APIs
  inline int getAPIs(const char **&idents)
  { idents = apiIDs_ ; return (apiIDCnt_) ; }
  /// Retrieve the object that implements a given API.
  inline void *getAPIPtr(const char *ident)
  { if (objForAPI_.find(ident) == objForAPI_.end())
    { return (nullptr) ; }
    else
    { return (objForAPI_.find(ident)->second) ; } }

  /// Add a parameter
  class ParamEntry ;
  void addParamID (const char *paramID, ParamEntry *entry) ;

private:

  /// Actual number of capabilities
  int apiIDCnt_ ;
  /// Length of capability ID vector
  int apiIDLen_ ;
  /// Vector of capability API IDs
  const char **apiIDs_ ;

  /// API to implementing object map

  std::map<std::string,void*> objForAPI_ ;

  /// Actual number of parameters
  int paramIDCnt_ ;
  /// Length of parameter ID vector
  int paramIDLen_ ;
  /// Vector of parameter IDs
  const char **paramIDs_ ;

  typedef std::map<std::string,ParamEntry *> EntryForParamMap ;
  /// Param ID to entry map
  EntryForParamMap entryForParam_ ;

  /// Indicator; false if the message handler belongs to the client
  bool dfltHandler_ ;
  /// Message handler
  CoinMessageHandler *msgHandler_ ;
  /// Messages
  CoinMessages msgs_ ;
  /// Log (verbosity) level
  int logLvl_ ;


  public: 
  class ParamEntry
  { 
    public:
      virtual ParamEntry *clone() = 0 ;
      virtual ~ParamEntry() { }
    protected:
    /// Name of the parameter
    std::string paramID_ ;
    public:
    /// Interface to the get method
    virtual bool get (Managed *obj, void *&blob) = 0 ;
    /// Interface to the set method
    virtual bool set (Managed *obj, const void *&blob) = 0 ;
  } ;


  template<class ValType>
  class ParamEntry_Imp : public ParamEntry
  { 
    public:
    typedef ValType (Managed::*GetFunc)() const ;
    typedef void (Managed::*SetFunc)(ValType val) ;

    GetFunc getFunc_ ;
    SetFunc setFunc_ ;

    ParamEntry_Imp (std::string id,
		    GetFunc getter, SetFunc setter)
    { ParamEntry::paramID_ = id ;
      getFunc_ = getter ;
      setFunc_ = setter ;
    } 

    ParamEntry_Imp (const ParamEntry_Imp &rhs)
    {
      ParamEntry::paramID_ = rhs.paramID_ ;
      getFunc_ = rhs.getFunc_ ;
      setFunc_ = rhs.setFunc_ ;
    }

    ParamEntry *clone ()
    {
      return (new ParamEntry_Imp(*this)) ;
    }

    ~ParamEntry_Imp () { } ;

    bool get (Managed *obj, void *&blob)
    {
      std::cout << "    invoking derived get." << std::endl ;
      ValType *val = static_cast<ValType *>(blob) ;
      *val = (obj->*getFunc_)() ;
      return (true) ;
    }

    bool set (Managed *obj, const void *&blob)
    {
      std::cout << "    invoking derived set." << std::endl ;
      const ValType *val = static_cast<const ValType *>(blob) ;
      (obj->*setFunc_)(*val) ;
      return (true) ;
    }
  } ;




} ;

} // namespace Osi2

/*
  And then we need the code, so that we have the definitions handy when
  instantiating a particular instance of the template.
*/
#include "Osi2ParamBEAPI_Imp.cpp"

#endif   // Osi2ParamBEAPI_Imp_HPP
