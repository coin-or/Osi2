/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$

  This file contains the implementation of the methods for the Osi2 ParamBE
  API.
*/

#include "Osi2Config.h"
#include "Osi2ParamBEAPI_Imp.hpp"
#include "Osi2ControlAPI.hpp"

namespace Osi2 {

template<class Managed>
void ParamBEAPI_Imp<Managed>::addAPIID (const char *apiID, void *obj)
{
  if (apiIDCnt_+1 > apiIDLen_) {
    const char **tmp = new const char*[apiIDLen_+5] ;
    for (int ndx = 0 ; ndx < apiIDCnt_ ; ndx++) { tmp[ndx] = apiIDs_[ndx] ; }
    delete[] apiIDs_ ;
    apiIDs_ = tmp ;
  }
  apiIDs_[apiIDCnt_++] = apiID ;
  objForAPI_[apiID] = obj ;

  return ;
}


template<class Managed>
void ParamBEAPI_Imp<Managed>::addParam (const char *paramID,
				        ParamEntry *entry)
{
  if (paramIDCnt_+1 > paramIDLen_) {
    const char **tmp = new const char*[paramIDLen_+5] ;
    for (int ndx = 0 ; ndx < paramIDCnt_ ; ndx++) {
      tmp[ndx] = paramIDs_[ndx] ;
    }
    delete[] paramIDs_ ;
    paramIDs_ = tmp ;
  }
  paramIDs_[paramIDCnt_++] = paramID ;
  entryForParam_[paramID] = entry ;

  return ;
}


/*
  It looks like the way to go here is a templated class, so that we can cast
  the object to the correct class and use a pointer to member function.
*/
template<class Managed>
bool ParamBEAPI_Imp<Managed>::get (const char *paramID, void *&blob)
{
  std::cout << "  Request to get parameter " << paramID << std::endl ;
  void *voidObj = getAPIPtr(Managed::getAPIIDString()) ;
  ParamEntry *paramEntry = entryForParam_[paramID] ;
  Managed *obj = reinterpret_cast<Managed *>(voidObj) ;
  paramEntry->get(obj,blob) ;
  return (true) ;
}


/*
  And a matching set method.
*/
template<class Managed>
bool ParamBEAPI_Imp<Managed>::set (const char *paramID, const void *&blob)
{
  std::cout << "  Request to set parameter " << paramID << std::endl ;
  void *voidObj = getAPIPtr(Managed::getAPIIDString()) ;
  ParamEntry *paramEntry = entryForParam_[paramID] ;
  Managed *obj = reinterpret_cast<Managed *>(voidObj) ;
  paramEntry->set(obj,blob) ;
  return (true) ;
}

/*
  Boilerplate: Constructors, destructors, & such like
*/

/*
  Default constructor
*/
template<class Managed>
ParamBEAPI_Imp<Managed>::ParamBEAPI_Imp ()
  : apiIDCnt_(0),
    apiIDLen_(0),
    apiIDs_(nullptr),
    paramIDCnt_(0),
    paramIDLen_(0),
    paramIDs_(nullptr),
    logLvl_(7)
{
    msgHandler_ = new CoinMessageHandler() ;
    msgs_ = ParamBEAPIMessages() ;
    msgHandler_->setLogLevel(logLvl_) ;
    msgHandler_->message(PMBEAPI_INIT, msgs_) << "default" << CoinMessageEol ;
}

/*
  Copy constructor
*/
template<class Managed>
ParamBEAPI_Imp<Managed>::ParamBEAPI_Imp (const ParamBEAPI_Imp &rhs)
  : dfltHandler_(rhs.dfltHandler_),
    logLvl_(rhs.logLvl_)
{
/*
  If we're holding an API ID vector, we need to replicate that.
*/
  apiIDCnt_ = rhs.apiIDCnt_ ;
  apiIDLen_ = rhs.apiIDLen_ ;
  apiIDs_ = new const char*[apiIDLen_] ;
  for (int ndx = 0 ; ndx < apiIDCnt_ ; ndx++) {
    apiIDs_[ndx] = rhs.apiIDs_[ndx] ;
  }
  objForAPI_ = rhs.objForAPI_ ;
/*
  Similarly for the param ID vector.
*/
  paramIDCnt_ = rhs.paramIDCnt_ ;
  paramIDLen_ = rhs.paramIDLen_ ;
  paramIDs_ = new const char*[paramIDLen_] ;
  for (int ndx = 0 ; ndx < paramIDCnt_ ; ndx++) {
    paramIDs_[ndx] = rhs.paramIDs_[ndx] ;
  }
  for (typename EntryForParamMap::const_iterator iter =
  		rhs.entryForParam_.begin() ;
       iter != rhs.entryForParam_.end() ;
       iter++) {
    ParamEntry *entry = iter->second->clone() ;
    entryForParam_[iter->first] = entry ;
  }
/*
  If this is our handler, make an independent copy. If it's the client's
  handler, we can't make an independent copy because the client won't know
  about it and won't delete it.
*/
  if (dfltHandler_) {
    msgHandler_ = new CoinMessageHandler(*rhs.msgHandler_) ;
  } else {
    msgHandler_ = rhs.msgHandler_ ;
  }
  msgs_ = rhs.msgs_ ;
  msgHandler_->setLogLevel(logLvl_) ;
  msgHandler_->message(PMBEAPI_INIT, msgs_) << "copy" << CoinMessageEol ;
}

/*
  Assignment
*/
template<class Managed>
ParamBEAPI_Imp<Managed> &ParamBEAPI_Imp<Managed>::operator=
    (const ParamBEAPI_Imp &rhs)
{
/*
  Self-assignment requires no work.
*/
  if (this == &rhs) return (*this) ;
/*
  If we're holding an API ID vector, we need to replicate that. The
  objForAPI_ map holds std::strings, so we can just do a direct assignment.
*/
  apiIDCnt_ = rhs.apiIDCnt_ ;
  apiIDLen_ = rhs.apiIDLen_ ;
  apiIDs_ = new const char*[apiIDLen_] ;
  for (int ndx = 0 ; ndx < apiIDCnt_ ; ndx++) {
    apiIDs_[ndx] = rhs.apiIDs_[ndx] ;
  }
  objForAPI_ = rhs.objForAPI_ ;
/*
  Similarly for the param ID vector. The entryForParm_ map is a different
  story. It holds pointers to ParamEntry objects that we need to replicate.
*/
  paramIDCnt_ = rhs.paramIDCnt_ ;
  paramIDLen_ = rhs.paramIDLen_ ;
  paramIDs_ = new const char*[paramIDLen_] ;
  for (int ndx = 0 ; ndx < paramIDCnt_ ; ndx++) {
    paramIDs_[ndx] = rhs.paramIDs_[ndx] ;
  }
  for (typename EntryForParamMap::const_iterator iter =
  		    rhs.entryForParam_.begin() ;
       iter != rhs.entryForParam_.end() ;
       iter++) {
    ParamEntry *entry = iter->second->clone() ;
    entryForParam_[iter->first] = entry ;
  }
/*
  If it's our handler, we need to delete the old and replace with the new.
  If it's the user's handler, it's the user's problem. We just assign the
  pointer.
*/
  if (dfltHandler_) {
    delete msgHandler_ ;
    msgHandler_ = nullptr ;
  }
  dfltHandler_ = rhs.dfltHandler_ ;
  if (dfltHandler_) {
    msgHandler_ = new CoinMessageHandler(*rhs.msgHandler_) ;
  } else {
    msgHandler_ = rhs.msgHandler_ ;
  }
  msgs_ = rhs.msgs_ ;
  msgHandler_->setLogLevel(logLvl_) ;

  return (*this) ;
}


/*
  Destructor
*/
template<class Managed>
ParamBEAPI_Imp<Managed>::~ParamBEAPI_Imp ()
{
/*
  If we're holding an API ID vector, delete it. We're not responsible for the
  character strings it points to, just the vector. Simiarly for the param IDs.
*/
  if (apiIDs_) { delete[] apiIDs_ ; }
  if (paramIDs_) { delete[] paramIDs_ ; }
/*
  We are responsible for the ParamEntry objects held in entryForParam_
*/
  typedef typename std::map<std::string,ParamEntry *>::iterator EFPIter ;
  for (EFPIter iter = entryForParam_.begin() ;
       iter != entryForParam_.end() ;
       iter++) {
    if (iter->second) { delete iter->second ; }
  }

/*
  If this is our handler, delete it. Otherwise it's the client's
  responsibility.
*/
  if (dfltHandler_) {
    delete msgHandler_ ;
    msgHandler_ = nullptr ;
  }
}

/*
  Virtual constructor
*/
template<class Managed>
ParamBEAPI *ParamBEAPI_Imp<Managed>::create ()
{
  ParamBEAPI *api = new ParamBEAPI_Imp() ;
  return (api) ;
}

/*
  Clone
*/
template<class Managed>
ParamBEAPI *ParamBEAPI_Imp<Managed>::clone ()
{
    ParamBEAPI *api = new ParamBEAPI_Imp(*this) ;
    return (api) ;
}


}    // namespace Osi2
