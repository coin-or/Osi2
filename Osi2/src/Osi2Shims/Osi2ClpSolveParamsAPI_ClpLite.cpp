/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ClpSolveParamsAPI_ClpLite.cpp
    \brief Method definitions for Osi2ClpSolveParamsAPI_ClpLite

  Method definitions for ClpSolveParamsAPI_ClpLite, an implementation of the
  problem management API using the `lite' clp shim that dynamically loads
  methods on demand.
*/

#include <iostream>
#include <cctype>

#include "Osi2DynamicLibrary.hpp"

#include "Osi2ClpSolveParamsAPI_ClpLite.hpp"

namespace {

#include "Osi2CFuncTemplates.cpp"

}

namespace Osi2 {

/*
  Static maps for known ClpSimplex integer, double, and string parameters. The
  first string is the name exposed through the parameter interface, the
  second a pointer to the data member that holds it.
*/

#define KnownParamsEntry(zzParam,zzMember) \
{ std::string(zzParam), &ClpSolveParamsAPI_ClpLite::zzMember }

ClpSolveParamsAPI_ClpLite::KnownParams<int>
ClpSolveParamsAPI_ClpLite::integerSimplexParams_ = {
  KnownParamsEntry("iter count",iterCount_),
  KnownParamsEntry("iter max",iterMax_),
  KnownParamsEntry("scaling",scaling_),
  KnownParamsEntry("perturbation",perturbation_),
  KnownParamsEntry("algorithm",algorithm_)
} ;
  

ClpSolveParamsAPI_ClpLite::KnownParams<double>
ClpSolveParamsAPI_ClpLite::doubleSimplexParams_ = {
  KnownParamsEntry("primal zero tol",primalZeroTol_),
  KnownParamsEntry("dual zero tol",dualZeroTol_),
  KnownParamsEntry("dual obj lim",dualObjLim_),
  KnownParamsEntry("obj offset",objOffset_),
  KnownParamsEntry("obj sense",objSense_),
  KnownParamsEntry("infeas cost",infeasCost_),
  KnownParamsEntry("small elem",smallElem_),
  KnownParamsEntry("dual bound",dualBound_)
} ;


ClpSolveParamsAPI_ClpLite::KnownParams<std::string>
ClpSolveParamsAPI_ClpLite::stringSimplexParams_ = {
  KnownParamsEntry("problem name",probName_)
} ;


/*
  Static map for known presolve transform names. The first string is the name
  exposed through the parameter interface, the second the name of the
  corresponding Clp_C_Interface ClpSolve method.
*/

typedef std::map<std::string,std::string>KnownXformMap ;
KnownXformMap knownXforms = {
  {std::string("return if infeasible"),std::string("infeasibleReturn")},
  {std::string("dual"),std::string("doDual")},
  {std::string("singleton"),std::string("doSingleton")},
  {std::string("doubleton"),std::string("doDoubleton")},
  {std::string("tripleton"),std::string("doTripleton")},
  {std::string("tighten"),std::string("doTighten")},
  {std::string("forcing"),std::string("doForcing")},
  {std::string("implied free"),std::string("doImpliedFree")},
  {std::string("dupcol"),std::string("doDupcol")},
  {std::string("duprow"),std::string("doDuprow")},
  {std::string("singleton column"),std::string("doSingletonColumn")},
  {std::string("kill small"),std::string("doKillSmall")}
} ;


/*
  Constructor

  \todo  Work up a constructor with reasonable defaults --- at the least,
  confirm that the defaults from ClpSolve are reasonable. Or figure out a way
  to load defaults from ClpSimplex.
*/
ClpSolveParamsAPI_ClpLite::ClpSolveParamsAPI_ClpLite (DynamicLibrary *libClp)
    : paramMgr_(ParamBEAPI_Imp<ClpSolveParamsAPI_ClpLite>(this)),
      libClp_(libClp),
      clpSolve_(nullptr)
{ 
  std::string errStr ;
/*
  Look up ClpSolve_new, coerce it into a suitable type, and invoke it to get a
  new Clp_Solve object.
*/
  typedef Clp_Solve *(*FactoryFunc)() ;
  FactoryFunc factoryFunc =
      libClp_->getFunc<FactoryFunc>(std::string("ClpSolve_new"),errStr) ;
  if (factoryFunc != nullptr) {
    clpSolve_ = factoryFunc() ;
  }
/*
  Register that we are a ClpSolveParams API and that we support parameter
  management.
*/
  paramMgr_.addAPIID(ClpSolveParamsAPI::getAPIIDString(),this) ;
  paramMgr_.addAPIID(ParamBEAPI::getAPIIDString(),&paramMgr_) ;
/*
  Register the parameters exposed through parameter management
*/
  typedef
    ParamBEAPI_Imp<ClpSolveParamsAPI_ClpLite>::ParamEntry_Imp<double> IDPE ;
  typedef
    ParamBEAPI_Imp<ClpSolveParamsAPI_ClpLite>::ParamEntry_Gen<std::string> SGPE ;
  ParamBEAPI_Imp<ClpSolveParamsAPI_ClpLite>::ParamEntry *paramEntry ;
  paramEntry = new IDPE("primal tolerance",
  		       &ClpSolveParamsAPI_ClpLite::primalTolerance,
  		       &ClpSolveParamsAPI_ClpLite::setPrimalTolerance) ;
  paramMgr_.addParam("primal tolerance",paramEntry) ;
  paramEntry = new IDPE("dual tolerance",
  		       &ClpSolveParamsAPI_ClpLite::dualTolerance,
  		       &ClpSolveParamsAPI_ClpLite::setDualTolerance) ;
  paramMgr_.addParam("dual tolerance",paramEntry) ;
  paramEntry = new SGPE("problem name","problem name",
	   &ClpSolveParamsAPI_ClpLite::getSimplexStrParam,
	   &ClpSolveParamsAPI_ClpLite::setSimplexStrParam) ;
  paramMgr_.addParam("problem name",paramEntry) ;
  return ;
}

ClpSolveParamsAPI_ClpLite::~ClpSolveParamsAPI_ClpLite ()
{
  std::string errStr ;
/*
  Look up ClpSolve_delete, coerce it into a suitable type, and invoke it to
  destroy the Clp_Solve object.
*/
  typedef void (*DeleteFunc)(Clp_Solve *clpSolve) ;
  DeleteFunc deleteFunc =
     libClp_->getFunc<DeleteFunc>("ClpSolve_delete",errStr) ;
  if (deleteFunc != nullptr) {
    deleteFunc(clpSolve_) ;
    clpSolve_ = nullptr ;
  }
}

/*
  Functions that act on the Clp_Solve object.
*/


void ClpSolveParamsAPI_ClpLite::setSpecialOption (int which,
					    int value, int extrainfo)
{
  simpleSetter<Clp_Solve,int>(libClp_,clpSolve_,"ClpSolve_setSpecialOption",
  			      which,value,extrainfo) ;
}

int ClpSolveParamsAPI_ClpLite::specialOption (int which) const
{
  return (simpleGetter<Clp_Solve,int>(libClp_,clpSolve_,
				      "ClpSolve_getSpecialOption",which)) ;
}

void ClpSolveParamsAPI_ClpLite::setSolveType (int method, int extrainfo)
{
  simpleSetter<Clp_Solve,int>(libClp_,clpSolve_,"ClpSolve_setSolveType",
  			      method,extrainfo) ;
}

int ClpSolveParamsAPI_ClpLite::solveType () const
{
  return (simpleGetter<Clp_Solve,int>(libClp_,clpSolve_,
  				      "ClpSolve_getSolveType")) ;
}

void ClpSolveParamsAPI_ClpLite::setPresolveType (int type, int passes)
{
  simpleSetter<Clp_Solve,int>(libClp_,clpSolve_,"ClpSolve_setPresolveType",
  			      type,passes) ;
}

int ClpSolveParamsAPI_ClpLite::presolveType () const
{
  return (simpleGetter<Clp_Solve,int>(libClp_,clpSolve_,
  				      "ClpSolve_getPresolveType")) ;
}

int ClpSolveParamsAPI_ClpLite::presolvePasses () const
{
  return (simpleGetter<Clp_Solve,int>(libClp_,clpSolve_,
				      "ClpSolve_getPresolvePasses")) ;
}

int ClpSolveParamsAPI_ClpLite::extraInfo (int which) const
{
  return (simpleGetter<Clp_Solve,int>(libClp_,clpSolve_,
				      "ClpSolve_getExtraInfo",which)) ;
}

void ClpSolveParamsAPI_ClpLite::setSubstitution (int val)
{
  simpleSetter<Clp_Solve,int>(libClp_,clpSolve_,"ClpSolve_setSubstitution",
  			      val) ;
}

int ClpSolveParamsAPI_ClpLite::substitution () const
{
  return (simpleGetter<Clp_Solve,int>(libClp_,clpSolve_,
  				      "ClpSolve_substitution")) ;
}

void ClpSolveParamsAPI_ClpLite::setPresolveXform (std::string xform,
						  bool onoff)
{
  KnownXformMap::const_iterator pxmIter = knownXforms.find(xform) ;
  if (pxmIter == knownXforms.end()) {
    std::cout
      << " Clp presolve xform " << xform << " is not recognised."
      << std::endl ;
    return ;
  }
  std::string tmp = pxmIter->second ;
  char ucChar = toupper(tmp.c_str()[0]) ;
  std::string setFuncName =  "ClpSolve_set"+ucChar+tmp.substr(1) ;
  simpleSetter<Clp_Solve,int>(libClp_,clpSolve_,setFuncName,onoff) ;
}

bool ClpSolveParamsAPI_ClpLite::presolveXform (std::string xform) const
{
  KnownXformMap::const_iterator pxmIter =
      knownXforms.find(xform) ;
  if (pxmIter == knownXforms.end()) {
    std::cout
      << " Clp presolve xform " << xform << " is not recognised."
      << std::endl ;
    return (false) ;
  }
  std::string getFuncName =  "ClpSolve_"+pxmIter->second ;
  bool retval = simpleGetter<Clp_Solve,bool>(libClp_,clpSolve_,getFuncName) ;
  return (retval) ;
}

template<class ValType>
void ClpSolveParamsAPI_ClpLite::setClpSimplexParam
  (KnownParams<ValType> knownMap, std::string param, ValType val)
{
  typename KnownParams<ValType>::const_iterator kpIter = knownMap.find(param) ;
  if (kpIter == knownMap.end()) {
    std::cout
      << " ClpSimplex parameter " << param << " is not recognised."
      << std::endl ;
    return ;
  }
  ValType ClpSolveParamsAPI_ClpLite:: *pmemb = kpIter->second ;
  (this->*pmemb) = val ;
}

template<class RetType>
RetType ClpSolveParamsAPI_ClpLite::getClpSimplexParam
  (KnownParams<RetType> knownMap, std::string param) const
{
  typename KnownParams<RetType>::const_iterator kpIter = knownMap.find(param) ;
  if (kpIter == knownMap.end()) {
    std::cout
      << " ClpSimplex parameter " << param << " is not recognised."
      << std::endl ;
    return (RetType()) ;
  }
  RetType ClpSolveParamsAPI_ClpLite:: *pmemb = kpIter->second ;
  return (this->*pmemb) ;
}


}    // end namespace Osi2
