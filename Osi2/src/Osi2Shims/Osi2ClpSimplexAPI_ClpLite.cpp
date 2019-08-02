/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ClpSimplexAPI_ClpLite.cpp
    \brief Method definitions for Osi2ClpSimplexAPI_ClpLite

  Method definitions for ClpSimplexAPI_ClpLite, an implementation of the
  problem management API using the `lite' clp shim that dynamically loads
  methods on demand.
*/

#include <iostream>

// #include "ClpConfig.h"
// #include "Clp_C_Interface.h"
// #include "Osi2ClpShim.hpp"

#include "Osi2Config.h"
#include "Osi2nullptr.hpp"


// #include "Osi2API.hpp"
// #include "Osi2ClpSimplexAPI.hpp"
#include "Osi2ClpSimplexAPI_ClpLite.hpp"

#include "Osi2DynamicLibrary.hpp"

namespace {

template <typename FuncType>
FuncType loadFunc (Osi2::DynamicLibrary *lib, const std::string funcName)
{
  std::string errStr ;
  /*
  FuncType func =
      reinterpret_cast<FuncType>(lib->getSymbol(funcName,errStr)) ;

  The code above is what should really happen. But ISO C++ forbids the
  conversion of a pointer-to-object to a pointer-to-function and there's no
  obvious way to suppress the warning with a compiler flag. On the other
  hand, we are allowed to cast pointer-to-anything to size_t and get it back.
  So we slip through a blind spot in the compiler's algorithm for generating
  warnings.
  */
  size_t grossHack = 
      reinterpret_cast<size_t>(lib->getSymbol(funcName,errStr)) ;
  FuncType func = reinterpret_cast<FuncType>(grossHack) ;
  if (func == nullptr) {
    std::cout << "Apparent failure to find " << funcName << "." << std::endl ;
    std::cout << errStr << std::endl ;
  }
  return (func) ;
}

}   // end unnamed file-local namespace

namespace Osi2 {

/*
  Capture a pointer to the underlying ClpSimplex object.
*/
ClpSimplexAPI_ClpLite::ClpSimplexAPI_ClpLite (DynamicLibrary *libClp)
    : libClp_(libClp),
      clpC_(nullptr)
{
/*
  Look up Clp_newModel, coerce it into a suitable type, and invoke it to get a
  new Clp_Simplex object.
*/
  typedef Clp_Simplex *(*ClpFactory)() ;
  ClpFactory factory =
      loadFunc<ClpFactory>(libClp,std::string("Clp_newModel")) ;
  if (factory != nullptr) {
    clpC_ = factory() ;
  }
  return ;
}

ClpSimplexAPI_ClpLite::~ClpSimplexAPI_ClpLite ()
{
/*
  Look up Clp_deleteModel, coerce it into a suitable type, and invoke it to
  destroy the Clp_Simplex object.
*/
  typedef void (*ClpDeleteModelFunc)(Clp_Simplex *clp) ;
  ClpDeleteModelFunc deleteModel =
     loadFunc<ClpDeleteModelFunc>(libClp_,"Clp_deleteModel") ;
  if (deleteModel != nullptr) {
    deleteModel(clpC_) ;
    clpC_ = nullptr ;
    std::cout << "Osi1API_ClpLite object destroyed." << std::endl ;
  }
}

/*
  Read a problem file in mps format.
*/
int ClpSimplexAPI_ClpLite::readMps (const char *filename, bool keepNames,
                              bool ignoreErrors)
{
  typedef int (*ClpReadMpsFunc)(Clp_Simplex *,const char *,int,int) ;
  static ClpReadMpsFunc readMps = nullptr ;

  if (readMps == nullptr) {
    readMps = loadFunc<ClpReadMpsFunc>(libClp_,"Clp_readMps") ;
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

/*
  Solve a problem
*/
int ClpSimplexAPI_ClpLite::initialSolve ()
{
  typedef int (*ClpInitSolFunc)(Clp_Simplex *) ;
  static ClpInitSolFunc initialSolve = nullptr ;

  if (initialSolve == nullptr) {
    ClpInitSolFunc initialSolve =
	  loadFunc<ClpInitSolFunc>(libClp_,"Clp_initialSolve") ;
  }
  int retval = -1 ;
  if (initialSolve != nullptr) {
      retval = initialSolve(clpC_) ;
      if (retval < 0) {
	  std::cout
	      << "Solve failed; error " << retval << "." << std::endl ;
      } else {
	  std::cout
	      << "Solved; return status " << retval << "." << std::endl ;
      }
  }
  return (retval) ;
}

}    // end namespace Osi2
