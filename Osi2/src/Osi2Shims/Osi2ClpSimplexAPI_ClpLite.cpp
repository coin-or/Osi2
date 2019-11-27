/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ClpSimplexAPI_ClpLite.cpp
    \brief Method definitions for Osi2ClpSimplexAPI_ClpLite

  Method definitions for ClpSimplexAPI_ClpLite, an implementation of the
  ClpSimplex API using the `lite' clp shim that dynamically loads methods
  on demand.
*/

#include <iostream>

#include "Osi2Config.h"
#include "Osi2nullptr.hpp"

#include "Osi2DynamicLibrary.hpp"
#include "Osi2RunParamsAPI.hpp"

#include "Osi2ClpSimplexAPI_ClpLite.hpp"

namespace {

// Templates for common function signatures

#include "Osi2CFuncTemplates.cpp"

/*
  Static maps for known ClpSimplex integer, double, and string parameters. The
  first string is the name exposed through Osi2 standard parameter interfaces,
  the second the name of the C interface get method, the third the name of
  the C interface set method.
*/

KnownMemberMap integerSimplexParams = {
  mapEntry3("iter count","Clp_numberIterations","Clp_setNumberIterations"),
  mapEntry3("iter max","Clp_maximumIterations","Clp_setMaximumIterations"),
  mapEntry3("scaling","Clp_scalingFlag","Clp_scaling"),
  mapEntry3("perturbation","Clp_perturbation","Clp_setPerturbation"),
  mapEntry3("algorithm","Clp_algorithm","Clp_setAlgorithm")
} ;
  

KnownMemberMap doubleSimplexParams = {
  mapEntry3("primal zero tol","Clp_primalTolerance","Clp_setPrimalTolerance"),
  mapEntry3("dual zero tol","Clp_dualTolerance","Clp_setDualTolerance"),
  mapEntry3("dual obj lim",
  	    "Clp_dualObjectiveLimit","Clp_setDualObjectiveLimit"),
  mapEntry3("obj offset","Clp_objectiveOffset","Clp_setObjectiveOffset"),
  mapEntry3("obj sense",
  	    "Clp_optimizationDirection","Clp_setOptimizationDirection"),
  mapEntry3("infeas cost","Clp_infeasibilityCost","Clp_setInfeasibilityCost"),
  mapEntry3("small elem",
  	    "Clp_getSmallElementValue","Clp_setSmallElementValue"),
  mapEntry3("dual bound","Clp_dualBound","Clp_setDualBound")
} ;

/*
  You'd expect Clp_problemName, Clp_setProblemName here but they don't fit the
  standard model for Clp_C_Interface get / set methods. An empty map is left
  for possible future additions.
*/
KnownMemberMap stringSimplexParams = { } ;


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

}    // end anonymous namespace


namespace Osi2 {

typedef ClpSimplexAPI_ClpLite CSA_CL ;

/*
  Constructor
*/
CSA_CL::ClpSimplexAPI_ClpLite (DynamicLibrary *libClp)
    : paramMgr_(ParamBEAPI_Imp<CSA_CL>(this)),
      libClp_(libClp),
      clpC_(nullptr)
{
  std::string errStr ;
/*
  Look up Clp_newModel, coerce it into a suitable type, and invoke it to get a
  new Clp_Simplex object.
*/
  typedef Clp_Simplex *(*ClpFactory)() ;
  ClpFactory factory =
      libClp_->getFunc<ClpFactory>(std::string("Clp_newModel"),errStr) ;
  if (factory != nullptr) {
    clpC_ = factory() ;
  }
/*
  Register that we are a ClpSimplex API and that we support parameter
  management.
*/
  paramMgr_.addAPIID(ClpSimplexAPI::getAPIIDString(),this) ;
  paramMgr_.addAPIID(ParamBEAPI::getAPIIDString(),&paramMgr_) ;
/*
  Register the parameters exposed through parameter management
*/
  typedef ParamBEAPI_Imp<CSA_CL>::ParamEntry_Imp<double> DPE ;
  typedef ParamBEAPI_Imp<CSA_CL>::ParamEntry_Imp<int> IPE ;
  typedef ParamBEAPI_Imp<CSA_CL>::ParamEntry_Void VPE ;

  ParamBEAPI_Imp<CSA_CL>::ParamEntry *paramEntry ;
  paramEntry = new DPE("primal tolerance",
  		       &CSA_CL::primalTolerance,
  		       &CSA_CL::setPrimalTolerance) ;
  paramMgr_.addParam("primal tolerance",paramEntry) ;
  paramEntry = new DPE("dual tolerance",
  		       &CSA_CL::dualTolerance,
  		       &CSA_CL::setDualTolerance) ;
  paramMgr_.addParam("dual tolerance",paramEntry) ;
  paramEntry = new DPE("dual obj limit",
  		       &CSA_CL::dualObjectiveLimit,
  		       &CSA_CL::setDualObjectiveLimit) ;
  paramMgr_.addParam("dual obj limit",paramEntry) ;
  paramEntry = new DPE("dual bound",
  		       &CSA_CL::dualObjectiveLimit,
  		       &CSA_CL::setDualObjectiveLimit) ;
  paramMgr_.addParam("dual bound",paramEntry) ;
  paramEntry = new IPE("scaling",
  		       &CSA_CL::scalingFlag,
  		       &CSA_CL::scaling) ;
  paramMgr_.addParam("scaling",paramEntry) ;
  paramEntry = new DPE("obj offset",
  		       &CSA_CL::objectiveOffset,
  		       &CSA_CL::setObjectiveOffset) ;
  paramMgr_.addParam("obj offset",paramEntry) ;
  paramEntry = new DPE("obj sense",
  		       &CSA_CL::objSense,
  		       &CSA_CL::setObjSense) ;
  paramMgr_.addParam("obj sense",paramEntry) ;
  paramEntry = new VPE("problem name",
  		       &CSA_CL::getProbNameVoid,
  		       &CSA_CL::setProbNameVoid) ;
  paramMgr_.addParam("problem name",paramEntry) ;
  paramEntry = new IPE("iter count",
  		       &CSA_CL::numberIterations,
  		       &CSA_CL::setNumberIterations) ;
  paramMgr_.addParam("iter count",paramEntry) ;
  paramEntry = new IPE("max iters",
  		       &CSA_CL::maximumIterations,
  		       &CSA_CL::setMaximumIterations) ;
  paramMgr_.addParam("max iters",paramEntry) ;
  paramEntry = new DPE("max secs",
  		       &CSA_CL::maximumSeconds,
  		       &CSA_CL::setMaximumSeconds) ;
  paramMgr_.addParam("max secs",paramEntry) ;
  paramEntry = new DPE("infeas cost",
  		       &CSA_CL::infeasibilityCost,
  		       &CSA_CL::setInfeasibilityCost) ;
  paramMgr_.addParam("infeas cost",paramEntry) ;
  paramEntry = new IPE("perturbation",
  		       &CSA_CL::perturbation,
  		       &CSA_CL::setPerturbation) ;
  paramMgr_.addParam("perturbation",paramEntry) ;
  paramEntry = new IPE("algorithm",
  		       &CSA_CL::algorithm,
  		       &CSA_CL::setAlgorithm) ;
  paramMgr_.addParam("algorithm",paramEntry) ;
  paramEntry = new DPE("small element",
  		       &CSA_CL::getSmallElementValue,
  		       &CSA_CL::setSmallElementValue) ;
  paramMgr_.addParam("small element",paramEntry) ;

  return ;
}

CSA_CL::~ClpSimplexAPI_ClpLite ()
{
  std::string errStr ;
/*
  Look up Clp_deleteModel, coerce it into a suitable type, and invoke it to
  destroy the Clp_Simplex object.
*/
  typedef void (*ClpDeleteModelFunc)(Clp_Simplex *clp) ;
  ClpDeleteModelFunc deleteModel =
     libClp_->getFunc<ClpDeleteModelFunc>("Clp_deleteModel",errStr) ;
  if (deleteModel != nullptr) {
    deleteModel(clpC_) ;
    clpC_ = nullptr ;
  }
}

/*
  Load a problem in packed matrix format.
*/
void CSA_CL::loadProblem (const int numcols, const int numrows,
	const CoinBigIndex *start, const int *index, const double *value,
	const double *collb, const double *colub,
	const double *obj,
	const double *rowlb, const double *rowub)
{
  std::string errStr ;

  typedef void (*ClpLdProbFunc)(Clp_Simplex *,const int, const int,
  	const CoinBigIndex *,const int *,const double *,
	const double *,const double *,const double *,
	const double *,const double *) ;
  static ClpLdProbFunc loadProblem = nullptr ;
  if (loadProblem == nullptr) {
    loadProblem = libClp_->getFunc<ClpLdProbFunc>("Clp_loadProblem",errStr) ;
  }
  if (loadProblem != nullptr) {
    loadProblem(clpC_,numcols,numrows,start,index,value,
    		collb,colub,obj,rowlb,rowub) ;
  }
}

/*
  Load the quadratic part of the objective (a matrix)
*/
void CSA_CL::loadQuadraticObjective (const int numcols,
	const CoinBigIndex *start, const int *index, const double *value)
{
  std::string errStr ;

  typedef void (*ClpLdQuadFunc)(Clp_Simplex *,const int,
  	const CoinBigIndex *,const int *,const double *) ;
  static ClpLdQuadFunc loadQuadObj = nullptr ;
  if (loadQuadObj == nullptr) {
    loadQuadObj =
        libClp_->getFunc<ClpLdQuadFunc>("Clp_loadQuadraticObjective",errStr) ;
  }
  if (loadQuadObj != nullptr) {
    loadQuadObj(clpC_,numcols,start,index,value) ;
  }
}

/*
  Load, retrieve, delete integer variable information.
*/

void CSA_CL::copyInIntegerInformation (const char *val)
{
  simpleSetter<Clp_Simplex,const char *>
      (libClp_,clpC_,"Clp_copyInIntegerInformation",val) ;
}

char *CSA_CL::integerInformation () const
{
  return (simpleGetter<Clp_Simplex,char *>
  	      (libClp_,clpC_,"Clp_integerInformation")) ;
}

void CSA_CL::deleteIntegerInformation ()
{
  simpleSetter<Clp_Simplex>(libClp_,clpC_,"Clp_deleteIntegerInformation") ;
}

void CSA_CL::resize (int newNumberRows, int newNumberColumns)
{
  simpleSetter<Clp_Simplex,int>(libClp_,clpC_,"Clp_resize",
  				newNumberRows,newNumberColumns) ;
}

/*
  Manipulate and retrieve information about rows
*/
int CSA_CL::numberRows ()
{ return (simpleGetter<Clp_Simplex,int>(libClp_,clpC_,"Clp_numberRows")) ; }

void CSA_CL::addRows (int number, const double *rowlb, const double *rowub,
	const CoinBigIndex *rowStart, const int *index, const double *value)
{
  std::string errStr ;

  typedef void (*ClpAddRowFunc)(Clp_Simplex *,
  	const int, const double *,const double *,
  	const CoinBigIndex *,const int *,const double *) ;
  static ClpAddRowFunc addRows = nullptr ;
  if (addRows == nullptr) {
    addRows =
        libClp_->getFunc<ClpAddRowFunc>("Clp_addRows",errStr) ;
  }
  if (addRows != nullptr) {
    addRows(clpC_,number,rowlb,rowub,rowStart,index,value) ;
  }
}

void CSA_CL::deleteRows (int number, const int *which)
{
  simpleSetter<Clp_Simplex,int,const int *>
      (libClp_,clpC_,"Clp_deleteRows",number,which) ;
}

double *CSA_CL::rowLower () const
{
  return (simpleGetter<Clp_Simplex,double *>(libClp_,clpC_,"Clp_rowLower")) ;
}

void CSA_CL::chgRowLower (const double *rowlb)
{
  simpleSetter<Clp_Simplex,const double *>
      (libClp_,clpC_,"Clp_chgRowLower",rowlb) ;
}

double *CSA_CL::rowUpper () const
{
  return (simpleGetter<Clp_Simplex,double *>(libClp_,clpC_,"Clp_rowUpper")) ;
}

void CSA_CL::chgRowUpper (const double *rowub)
{
  simpleSetter<Clp_Simplex,const double *>
      (libClp_,clpC_,"Clp_chgRowUpper",rowub) ;
}


/*
  Manipulate and retrieve information about columns
*/
int CSA_CL::numberColumns ()
{ return (simpleGetter<Clp_Simplex,int>(libClp_,clpC_,"Clp_numberColumns")) ; }

void CSA_CL::addColumns (int number,
	const double *collb, const double *colub, const double *obj,
	const CoinBigIndex *colStart, const int *index, const double *value)
{
  std::string errStr ;

  typedef void (*ClpAddColFunc)(Clp_Simplex *,
  	const int, const double *,const double *, const double *,
  	const CoinBigIndex *,const int *,const double *) ;
  static ClpAddColFunc addCols = nullptr ;
  if (addCols == nullptr) {
    addCols =
        libClp_->getFunc<ClpAddColFunc>("Clp_addColumns",errStr) ;
  }
  if (addCols != nullptr) {
    addCols(clpC_,number,collb,colub,obj,colStart,index,value) ;
  }
}

void CSA_CL::deleteColumns (int number, const int *which)
{
  simpleSetter<Clp_Simplex,int,const int *>
      (libClp_,clpC_,"Clp_deleteColumns",number,which) ;
}

double *CSA_CL::columnLower () const
{
  return (simpleGetter<Clp_Simplex,double *>
  	      (libClp_,clpC_,"Clp_columnLower")) ;
}

void CSA_CL::chgColumnLower (const double *collb)
{
  simpleSetter<Clp_Simplex,const double *>
      (libClp_,clpC_,"Clp_chgColumnLower",collb) ;
}

double *CSA_CL::columnUpper () const
{
  return (simpleGetter<Clp_Simplex,double *>
  	      (libClp_,clpC_,"Clp_columnUpper")) ;
}

void CSA_CL::chgColumnUpper (const double *colub)
{
  simpleSetter<Clp_Simplex,const double *>
      (libClp_,clpC_,"Clp_chgColumnUpper",colub) ;
}

double *CSA_CL::objective () const
{
  return (simpleGetter<Clp_Simplex,double *>(libClp_,clpC_,"Clp_objective")) ;
}

void CSA_CL::chgObjCoefficients (const double *obj)
{
  simpleSetter<Clp_Simplex,const double *>
      (libClp_,clpC_,"Clp_chgObjCoefficients",obj) ;
}

/*
  Retrieve information about the constraint matrix.
*/
CoinBigIndex CSA_CL::getNumElements () const
{ 
  return (simpleGetter<Clp_Simplex,CoinBigIndex>
	      (libClp_,clpC_,"Clp_getNumElements")) ;
}

const CoinBigIndex *CSA_CL::getVectorStarts () const
{ 
  return (simpleGetter<Clp_Simplex,const CoinBigIndex *>
	      (libClp_,clpC_,"Clp_getVectorStarts")) ;
}

const int *CSA_CL::getVectorLengths () const
{ 
  return (simpleGetter<Clp_Simplex,const int *>
	      (libClp_,clpC_,"Clp_getVectorLengths")) ;
}

const int *CSA_CL::getIndices () const
{ 
  return (simpleGetter<Clp_Simplex,const int *>
	      (libClp_,clpC_,"Clp_getIndices")) ;
}

const double *CSA_CL::getElements () const
{ 
  return (simpleGetter<Clp_Simplex,const double *>
	      (libClp_,clpC_,"Clp_getElements")) ;
}

void CSA_CL::modifyCoefficient (int row, int col, double val, bool keepZero)
{
  std::string errStr ;

  typedef void (*ClpModCoeffFunc)(Clp_Simplex *,int,int,double,bool) ;
  static ClpModCoeffFunc modCoeff = nullptr ;

  if (modCoeff == nullptr) {
    modCoeff =
        libClp_->getFunc<ClpModCoeffFunc>("Clp_modifyCoefficient",errStr) ;
  }
  if (modCoeff != nullptr) {
    modCoeff(clpC_,row,col,val,keepZero) ;
  }
}

/*
  Retrieve and modify variable status information.
*/
bool CSA_CL::statusExists () const
{
  return (simpleGetter<Clp_Simplex,bool>(libClp_,clpC_,"Clp_statusExists")) ;
}

unsigned char *CSA_CL::statusArray () const
{ 
  return (simpleGetter<Clp_Simplex,unsigned char *>
	      (libClp_,clpC_,"Clp_statusArray")) ;
}

void CSA_CL::copyinStatus (const unsigned char *status)
{
  simpleSetter<Clp_Simplex,const unsigned char *>
      (libClp_,clpC_,"Clp_copyinStatus",status) ;
}

ClpSimplexAPI::Status CSA_CL::getColumnStatus (int ndx)
{ 
  return (simpleGetter<Clp_Simplex,ClpSimplexAPI::Status,int>
	      (libClp_,clpC_,"Clp_getColumnStatus",ndx)) ;
}

void CSA_CL::setColumnStatus (int ndx, ClpSimplexAPI::Status status)
{
  simpleSetter<Clp_Simplex,int,ClpSimplexAPI::Status>
      (libClp_,clpC_,"Clp_setColumnStatus",ndx,status) ;
}

ClpSimplexAPI::Status CSA_CL::getRowStatus (int ndx)
{ 
  return (simpleGetter<Clp_Simplex,ClpSimplexAPI::Status,int>
	      (libClp_,clpC_,"Clp_getRowStatus",ndx)) ;
}

void CSA_CL::setRowStatus (int ndx, ClpSimplexAPI::Status status)
{
  simpleSetter<Clp_Simplex,int,ClpSimplexAPI::Status>
      (libClp_,clpC_,"Clp_setRowStatus",ndx,status) ;
}

/*
  Retrieve and modify row and column names.
*/
int CSA_CL::lengthNames () const
{
  return (simpleGetter<Clp_Simplex,int>(libClp_,clpC_,"Clp_lengthNames")) ;
}

void CSA_CL::rowName (int ndx, char *buffer) const
{
  simpleSetter<Clp_Simplex,int,char *>
      (libClp_,clpC_,"Clp_rowName",ndx,buffer) ;
}

void CSA_CL::setRowName (int ndx, const char *buffer)
{
  simpleSetter<Clp_Simplex,int,const char *>
      (libClp_,clpC_,"Clp_setRowName",ndx,buffer) ;
}

void CSA_CL::columnName (int ndx, char *buffer) const
{
  simpleSetter<Clp_Simplex,int,char *>
      (libClp_,clpC_,"Clp_columnName",ndx,buffer) ;
}

void CSA_CL::setColumnName (int ndx, const char *buffer)
{
  simpleSetter<Clp_Simplex,int,const char *>
      (libClp_,clpC_,"Clp_setColumnName",ndx,buffer) ;
}

void CSA_CL::copyNames (const char *const *rowNames,
			const char *const *columnNames)
{
  simpleSetter<Clp_Simplex,const char *const *,const char *const *>
      (libClp_,clpC_,"Clp_copyNames",rowNames,columnNames) ;
}

void CSA_CL::dropNames ()
{
  simpleSetter<Clp_Simplex>(libClp_,clpC_,"Clp_dropNames") ;
}

/*
  Read/write problems from/to a file.
*/
int CSA_CL::readMps (const char *filename, bool keepNames, bool ignoreErrors)
{
  std::string errStr ;

  typedef int (*ClpReadMpsFunc)(Clp_Simplex *,const char *,int,int) ;
  static ClpReadMpsFunc readMps = nullptr ;

  if (readMps == nullptr) {
    readMps = libClp_->getFunc<ClpReadMpsFunc>("Clp_readMps",errStr) ;
  }
  int retval = -1 ;
  if (readMps != nullptr) {
    retval = readMps(clpC_,filename,keepNames,ignoreErrors) ;
    if (retval) {
	std::cout
	    << "Failure to read " << filename << ", error " << retval
	    << "." << std::endl ;
    } else {
	std::cout
	    << "Read " << filename << " without error." << std::endl ;
    }
  }
  return (retval) ;
}

int CSA_CL::writeMps (const char *filename, int formatType, int numberAcross,
		      double objSense)
{
  std::string errStr ;

  typedef int (*ClpWriteMpsFunc)(Clp_Simplex *,const char *,int,int,double) ;
  static ClpWriteMpsFunc writeMps = nullptr ;

  if (writeMps == nullptr) {
    writeMps = libClp_->getFunc<ClpWriteMpsFunc>("Clp_writeMps",errStr) ;
  }
  int retval = -1 ;
  if (writeMps != nullptr) {
    retval = writeMps(clpC_,filename,formatType,numberAcross,objSense) ;
    if (retval) {
	std::cout
	    << "Failure to write " << filename << ", error " << retval
	    << "." << std::endl ;
    } else {
	std::cout
	    << "Wrote " << filename << " without error." << std::endl ;
    }
  }
  return (retval) ;
}

int CSA_CL::saveModel (const char *fileName)
{
  return (simpleGetter<Clp_Simplex,int,const char *>
  	      (libClp_,clpC_,"Clp_saveModel",fileName)) ;
}

int CSA_CL::restoreModel (const char *fileName)
{
  return (simpleGetter<Clp_Simplex,int,const char *>
  	      (libClp_,clpC_,"Clp_restoreModel",fileName)) ;
}



/* Individual parameter set / get methods */

double CSA_CL::primalTolerance () const
{
  return (simpleGetter<Clp_Simplex,double>
	      (libClp_,clpC_,"Clp_primalTolerance")) ;
}
void CSA_CL::setPrimalTolerance (double val)
{
  simpleSetter<Clp_Simplex,double>
      (libClp_,clpC_,"Clp_setPrimalTolerance",val) ;
}

double CSA_CL::dualTolerance () const
{
  return (simpleGetter<Clp_Simplex,double>
	      (libClp_,clpC_,"Clp_dualTolerance")) ;
}
void CSA_CL::setDualTolerance (double val)
{
  simpleSetter<Clp_Simplex,double>
      (libClp_,clpC_,"Clp_setDualObjectiveLimit",val) ;
}

double CSA_CL::dualObjectiveLimit () const
{
  return (simpleGetter<Clp_Simplex,double>
	      (libClp_,clpC_,"Clp_dualObjectiveLimit")) ;
}
void CSA_CL::setDualObjectiveLimit (double val)
{
  simpleSetter<Clp_Simplex,double>
      (libClp_,clpC_,"Clp_setDualTolerance",val) ;
}

double CSA_CL::dualBound () const
{
  return (simpleGetter<Clp_Simplex,double>
	      (libClp_,clpC_,"Clp_dualBound")) ;
}
void CSA_CL::setDualBound (double val)
{
  simpleSetter<Clp_Simplex,double>
      (libClp_,clpC_,"Clp_setBound",val) ;
}

int CSA_CL::scalingFlag () const
{
  return (simpleGetter<Clp_Simplex,int>
	      (libClp_,clpC_,"Clp_scalingFlag")) ;
}
void CSA_CL::scaling (int val)
{
  simpleSetter<Clp_Simplex,int>
      (libClp_,clpC_,"Clp_scaling",val) ;
}

double CSA_CL::objectiveOffset () const
{
  return (simpleGetter<Clp_Simplex,double>
	      (libClp_,clpC_,"Clp_objectiveOffset")) ;
}
void CSA_CL::setObjectiveOffset (double val)
{
  simpleSetter<Clp_Simplex,double>
      (libClp_,clpC_,"Clp_setObjectiveOffset",val) ;
}

double CSA_CL::objSense () const
{
  return (simpleGetter<Clp_Simplex,double>
	      (libClp_,clpC_,"Clp_optimizationDirection")) ;
}
void CSA_CL::setObjSense (double val)
{
  simpleSetter<Clp_Simplex,double>
      (libClp_,clpC_,"Clp_setOptimizationDirection",val) ;
}

/*
  Set / get problem name. The standard ClpSimplex methods don't find the
  parameter models used by RunParamsAPI or ParamBEAPI so define some wrappers.

  Clp_problemName returns a null-terminated string in buffer.
  CSA_CL::problemName counts on automatic construction of a std::string from a
  null-terminated string.

  Clp_setProblemName requires a null-terminated string in buffer and
  ignores buflen.  The value returned by Clp_setProblemName is actually a
  bool meant to indicate if the parameter was successfully set. The template
  needs to know there's a return value, but it's pointless to pass it on.

  There are two styles here (std::string return value, and data blob) as an
  exercise to make sure different code paths are working.
*/
std::string CSA_CL::problemName () const
{
  const int buflen = 256 ;
  char buffer[buflen] ;
  simpleSetter<Clp_Simplex,int,char *>
      (libClp_,clpC_,"Clp_problemName",buflen,buffer) ;
  return (buffer) ;
}
void CSA_CL::setProblemName (std::string name)
{
  int buflen = name.length() ;
  simpleGetter<Clp_Simplex,int,int,const char *>
      (libClp_,clpC_,"Clp_setProblemName",buflen,name.c_str()) ;
}

// Wrappers for ParamBEAPI, using a complex blob for data

void CSA_CL::getProbNameVoid (void *blob) const
{
  struct ProbNameParam { int buflen_ ; char *buffer_ ; } ;
  ProbNameParam *param = reinterpret_cast<ProbNameParam *>(blob) ;
  simpleSetter<Clp_Simplex,int,char *>
      (libClp_,clpC_,"Clp_problemName",param->buflen_,param->buffer_) ;
}

void CSA_CL::setProbNameVoid (const void *blob)
{
  struct ProbNameParam { int buflen_ ; char *buffer_ ; } ;
  const ProbNameParam *param = reinterpret_cast<const ProbNameParam *>(blob) ;
  simpleGetter<Clp_Simplex,int,int,const char *>
      (libClp_,clpC_,"Clp_setProblemName",param->buflen_,param->buffer_) ;
}

int CSA_CL::numberIterations () const
{
  return (simpleGetter<Clp_Simplex,int>
	      (libClp_,clpC_,"Clp_numberIterations")) ;
}
void CSA_CL::setNumberIterations (int val)
{
  simpleSetter<Clp_Simplex,int>
      (libClp_,clpC_,"Clp_setNumberIterations",val) ;
}

int CSA_CL::maximumIterations () const
{
  return (simpleGetter<Clp_Simplex,int>
	      (libClp_,clpC_,"Clp_maximumIterations")) ;
}
void CSA_CL::setMaximumIterations (int val)
{
  simpleSetter<Clp_Simplex,int>
      (libClp_,clpC_,"Clp_setMaximumIterations",val) ;
}

double CSA_CL::maximumSeconds () const
{
  return (simpleGetter<Clp_Simplex,double>
	      (libClp_,clpC_,"Clp_maximumSeconds")) ;
}
void CSA_CL::setMaximumSeconds (double val)
{
  simpleSetter<Clp_Simplex,double>
      (libClp_,clpC_,"Clp_setMaximumSeconds",val) ;
}

double CSA_CL::infeasibilityCost () const
{
  return (simpleGetter<Clp_Simplex,double>
	      (libClp_,clpC_,"Clp_infeasibilityCost")) ;
}
void CSA_CL::setInfeasibilityCost (double val)
{
  simpleSetter<Clp_Simplex,double>
      (libClp_,clpC_,"Clp_setInfeasibilityCost",val) ;
}

int CSA_CL::perturbation () const
{
  return (simpleGetter<Clp_Simplex,int>
	      (libClp_,clpC_,"Clp_maximumIterations")) ;
}
void CSA_CL::setPerturbation (int val)
{
  simpleSetter<Clp_Simplex,int>
      (libClp_,clpC_,"Clp_setPerturbation",val) ;
}

int CSA_CL::algorithm () const
{
  return (simpleGetter<Clp_Simplex,int>
	      (libClp_,clpC_,"Clp_maximumIterations")) ;
}
void CSA_CL::setAlgorithm (int val)
{
  simpleSetter<Clp_Simplex,int>
      (libClp_,clpC_,"Clp_setAlgorithm",val) ;
}

double CSA_CL::getSmallElementValue () const
{
  return (simpleGetter<Clp_Simplex,double>
	      (libClp_,clpC_,"Clp_infeasibilityCost")) ;
}
void CSA_CL::setSmallElementValue (double val)
{
  simpleSetter<Clp_Simplex,double>
      (libClp_,clpC_,"Clp_setSmallElementValue",val) ;
}


/*
  Generic parameter set / get methods
*/

int CSA_CL::getIntParam (std::string name) const
{
  return (getMember<Clp_Simplex,int>
	      (integerSimplexParams,name,libClp_,clpC_)) ;
}
double CSA_CL::getDblParam (std::string name) const
{
  return (getMember<Clp_Simplex,double>
	      (doubleSimplexParams,name,libClp_,clpC_)) ;
}
std::string CSA_CL::getStrParam (std::string name) const
{
  return (getMember<Clp_Simplex,std::string>
	      (stringSimplexParams,name,libClp_,clpC_)) ;
}

void CSA_CL::setIntParam (std::string name, int val)
{
  setMember<Clp_Simplex,int>
      (integerSimplexParams,name,libClp_,clpC_,val) ;
}
void CSA_CL::setDblParam (std::string name, double val)
{
  setMember<Clp_Simplex,double>
      (doubleSimplexParams,name,libClp_,clpC_,val) ;
}
void CSA_CL::setStrParam (std::string name, std::string val)
{
  setMember<Clp_Simplex,std::string>
      (stringSimplexParams,name,libClp_,clpC_,val) ;
}

/*
  Methods to work with RunParamsAPI objects.
*/
void CSA_CL::exposeParams (RunParamsAPI &runParams) const
{
  std::string name ;
  std::string getter ;

  for (KnownMemberMap::const_iterator iter = integerSimplexParams.begin() ;
       iter != integerSimplexParams.end() ;
       iter++) {
    name = iter->first ;
    getter = iter->second.getter_ ;
    int val = simpleGetter<Clp_Simplex,int>(libClp_,clpC_,getter) ;
    runParams.addIntParam(name,val) ;
  }
  for (KnownMemberMap::const_iterator iter = doubleSimplexParams.begin() ;
       iter != doubleSimplexParams.end() ;
       iter++) {
    name = iter->first ;
    getter = iter->second.getter_ ;
    double val = simpleGetter<Clp_Simplex,double>(libClp_,clpC_,getter) ;
    runParams.addDblParam(name,val) ;
  }
  for (KnownMemberMap::const_iterator iter = stringSimplexParams.begin() ;
       iter != stringSimplexParams.end() ;
       iter++) {
    name = iter->first ;
    getter = iter->second.getter_ ;
    std::string val =
        simpleGetter<Clp_Simplex,std::string>(libClp_,clpC_,getter) ;
    runParams.addStrParam(name,val) ;
  }
/*
  Special case for problem name, which uses local wrappers.
*/
  name = "problem name" ;
  std::string val = problemName() ;
  runParams.addStrParam(name,val) ;
}

void CSA_CL::loadParams (RunParamsAPI &runParams)
{
  std::vector<std::string> paramNames = runParams.getIntParamIds() ;
  for (std::vector<std::string>::const_iterator iter = paramNames.begin() ;
       iter != paramNames.end() ;
       iter++) {
    std::string name = *iter ;
    int val = runParams.getIntParam(name) ;
    std::string setFunc = integerSimplexParams[name].setter_ ;
    simpleSetter<Clp_Simplex,int>(libClp_,clpC_,setFunc,val) ;
  }
  paramNames = runParams.getDblParamIds() ;
  for (std::vector<std::string>::const_iterator iter = paramNames.begin() ;
       iter != paramNames.end() ;
       iter++) {
    std::string name = *iter ;
    double val = runParams.getDblParam(name) ;
    std::string setFunc = doubleSimplexParams[name].setter_ ;
    simpleSetter<Clp_Simplex,double>(libClp_,clpC_,setFunc,val) ;
  }
  paramNames = runParams.getStrParamIds() ;
  for (std::vector<std::string>::const_iterator iter = paramNames.begin() ;
       iter != paramNames.end() ;
       iter++) {
    std::string name = *iter ;
    std::string val = runParams.getStrParam(name) ;
    if (name != "problem name") {
      std::string setFunc = stringSimplexParams[name].setter_ ;
      simpleSetter<Clp_Simplex,std::string>(libClp_,clpC_,setFunc,val) ;
    } else {
      setProblemName(val) ;
    }
  }
}

/*
  Methods to get information about the solution.
*/

int CSA_CL::status () const
{
  return (simpleGetter<Clp_Simplex,int>
	      (libClp_,clpC_,"Clp_status")) ;
}
void CSA_CL::setProblemStatus (int val)
{
  simpleSetter<Clp_Simplex,int>
      (libClp_,clpC_,"Clp_setProblemStatus",val) ;
}

int CSA_CL::secondaryStatus () const
{
  return (simpleGetter<Clp_Simplex,int>
	      (libClp_,clpC_,"Clp_secondaryStatus")) ;
}
void CSA_CL::setSecondaryStatus (int val)
{
  simpleSetter<Clp_Simplex,int>
      (libClp_,clpC_,"Clp_setSecondaryStatus",val) ;
}

bool CSA_CL::primalFeasible () const
{
  return (simpleGetter<Clp_Simplex,bool>
	      (libClp_,clpC_,"Clp_primalFeasible")) ;
}

bool CSA_CL::dualFeasible () const
{
  return (simpleGetter<Clp_Simplex,bool>
	      (libClp_,clpC_,"Clp_dualFeasible")) ;
}

bool CSA_CL::isAbandoned () const
{
  return (simpleGetter<Clp_Simplex,bool>
	      (libClp_,clpC_,"Clp_isAbandoned")) ;
}

bool CSA_CL::hitMaximumIterations () const
{
  return (simpleGetter<Clp_Simplex,bool>
	      (libClp_,clpC_,"Clp_hitMaximumIterations")) ;
}

bool CSA_CL::isIterationLimitReached () const
{
  return (simpleGetter<Clp_Simplex,bool>
	      (libClp_,clpC_,"Clp_isIterationLimitReached")) ;
}

bool CSA_CL::isProvenOptimal () const
{
  return (simpleGetter<Clp_Simplex,bool>
	      (libClp_,clpC_,"Clp_isProvenOptimal")) ;
}

bool CSA_CL::isProvenPrimalInfeasible () const
{
  return (simpleGetter<Clp_Simplex,bool>
	      (libClp_,clpC_,"Clp_isAbandoned")) ;
}

bool CSA_CL::isProvenDualInfeasible () const
{
  return (simpleGetter<Clp_Simplex,bool>
	      (libClp_,clpC_,"Clp_isProvenDualInfeasible")) ;
}

double CSA_CL::objectiveValue() const
{
  return (simpleGetter<Clp_Simplex,double>
              (libClp_,clpC_,"Clp_objectiveValue")) ;
}

const double *CSA_CL::getRowActivity() const
{
  return (simpleGetter<Clp_Simplex,const double *>
              (libClp_,clpC_,"Clp_getRowActivity")) ;
}

const double *CSA_CL::getColSolution() const
{
  return (simpleGetter<Clp_Simplex,const double *>
              (libClp_,clpC_,"Clp_getColSolution")) ;
}

void CSA_CL::setColSolution(const double *soln)
{
  simpleSetter<Clp_Simplex,const double *>
              (libClp_,clpC_,"Clp_setColSolution",soln) ;
}

const double *CSA_CL::getRowPrice() const
{
  return (simpleGetter<Clp_Simplex,const double *>
              (libClp_,clpC_,"Clp_getRowPrice")) ;
}

const double *CSA_CL::getReducedCost() const
{
  return (simpleGetter<Clp_Simplex,const double *>
              (libClp_,clpC_,"Clp_getReducedCost")) ;
}

double *CSA_CL::unboundedRay() const
{
  return (simpleGetter<Clp_Simplex,double *>
              (libClp_,clpC_,"Clp_unboundedRay")) ;
}

double *CSA_CL::infeasibilityRay() const
{
  return (simpleGetter<Clp_Simplex,double *>
              (libClp_,clpC_,"Clp_infeasibilityRay")) ;
}

void CSA_CL::freeRay(double *ray) const
{
  simpleSetter<Clp_Simplex,double *>(libClp_,clpC_,"Clp_freeRay",ray) ;
}

int CSA_CL::numberPrimalInfeasibilities() const
{
  return (simpleGetter<Clp_Simplex,int>
              (libClp_,clpC_,"Clp_numberPrimalInfeasibilities")) ;
}

double CSA_CL::sumPrimalInfeasibilities() const
{
  return (simpleGetter<Clp_Simplex,double>
              (libClp_,clpC_,"Clp_sumPrimalInfeasibilities")) ;
}

int CSA_CL::numberDualInfeasibilities() const
{
  return (simpleGetter<Clp_Simplex,int>
              (libClp_,clpC_,"Clp_numberDualInfeasibilities")) ;
}

double CSA_CL::sumDualInfeasibilities() const
{
  return (simpleGetter<Clp_Simplex,double>
              (libClp_,clpC_,"Clp_sumDualInfeasibilities")) ;
}

void CSA_CL::checkSolution()
{
  simpleSetter<Clp_Simplex>(libClp_,clpC_,"Clp_checkSolution") ;
}

/*
  User pointer
*/

void *CSA_CL::getUserPointer() const
{
  return (simpleGetter<Clp_Simplex,void *>
              (libClp_,clpC_,"Clp_getUserPointer")) ;
}

void CSA_CL::setUserPointer(void *pointer)
{
  simpleSetter<Clp_Simplex>(libClp_,clpC_,"Clp_setUserPointer",pointer) ;
}

/*
  Message handling with callbacks
*/
void CSA_CL::registerCallBack (CallBack userCallBack)
{
  simpleSetter<Clp_Simplex,CallBack>
      (libClp_,clpC_,"Clp_registerCallBack",userCallBack) ;
}

void CSA_CL::clearCallBack ()
{
  simpleSetter<Clp_Simplex>(libClp_,clpC_,"Clp_registerCallBack") ;
}

void CSA_CL::setLogLevel (int level)
{
  simpleSetter<Clp_Simplex,int>(libClp_,clpC_,"Clp_setLogLevel",level) ;
}

int CSA_CL::logLevel () const
{
  return (simpleGetter<Clp_Simplex,int>
              (libClp_,clpC_,"Clp_logLevel")) ;
}

/*
  Solve a problem
*/
int CSA_CL::initialSolve ()
{
  return (simpleGetter<Clp_Simplex,int>(libClp_,clpC_,"Clp_initialSolve")) ;
}

int CSA_CL::initialDualSolve ()
{
  return (simpleGetter<Clp_Simplex,int>(libClp_,clpC_,"Clp_initialDualSolve")) ;
}

int CSA_CL::initialPrimalSolve ()
{
  return (simpleGetter<Clp_Simplex,int>
              (libClp_,clpC_,"Clp_initialPrimalSolve")) ;
}

int CSA_CL::initialBarrierSolve ()
{
  return (simpleGetter<Clp_Simplex,int>
              (libClp_,clpC_,"Clp_initialBarrierSolve")) ;
}

int CSA_CL::initialBarrierNoCrossSolve ()
{
  return (simpleGetter<Clp_Simplex,int>
              (libClp_,clpC_,"Clp_initialBarrierNoCrossSolve")) ;
}

int CSA_CL::dual (int ifValuesPass)
{
  return (simpleGetter<Clp_Simplex,int,int>
  	      (libClp_,clpC_,"Clp_dual",ifValuesPass)) ;
}

int CSA_CL::primal (int ifValuesPass)
{
  return (simpleGetter<Clp_Simplex,int,int>
  	      (libClp_,clpC_,"Clp_primal",ifValuesPass)) ;
}

int CSA_CL::crash (double gap, int pivot)
{
  return (simpleGetter<Clp_Simplex,int,double,int>
  	      (libClp_,clpC_,"Clp_primal",gap,pivot)) ;
}


}    // end namespace Osi2

