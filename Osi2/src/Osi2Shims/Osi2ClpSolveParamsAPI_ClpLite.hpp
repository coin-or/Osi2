/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ClpSolveParamsAPI_ClpLite.hpp
    \brief The clp `lite' implementation of Osi2::ClpSolveParamsAPI
*/
#ifndef Osi2ClpSolveParamsAPI_ClpLite_HPP
#define Osi2ClpSolveParamsAPI_ClpLite_HPP

#include "Osi2API.hpp"
#include "Osi2ClpSolveParamsAPI.hpp"

#include "Osi2ParamBEAPI_Imp.hpp"

#include "Clp_C_Interface.h"

namespace Osi2 {

/*! \brief Solve time parameter object for clp.

  The idea is that one might want to solve a model with different parameters.
  A user can establish several parameter objects with different values and
  solve a model repeatedly with different parameter objects.
*/
class ClpSolveParamsAPI_ClpLite : public ClpSolveParamsAPI {

public:
/*! \name Constructors, destructor, and copy */
//@{
  /// Constructor
  ClpSolveParamsAPI_ClpLite(DynamicLibrary *libClp) ;

  /// Destructor
  ~ClpSolveParamsAPI_ClpLite() ;
//@}

/*! \name ClpSimplex parameter gets & sets

  These are parameters normally exposed through the ClpSimplex interface.
  Note that invoking the methods here will change values in the
  ClpSolveParamsAPI object but these changes will not be applied to the
  underlying clp solver until a solve is requested.
*/
//@{
  /// Primal zero tolerance
  inline double primalTolerance() const { return (primalZeroTol_) ; }
  inline void setPrimalTolerance(double val) { primalZeroTol_ = val ; }

  /// Dual zero tolerance
  double dualTolerance() const { return (dualZeroTol_) ; }
  void setDualTolerance(double val) { dualZeroTol_ = val ; }

  /// Friendly generic get / set methods
  std::string getSimplexStrParam(std::string what) const
  {
    return (getClpSimplexParam<std::string>(stringSimplexParams_,what)) ;
  }
  void setSimplexStrParam(std::string what,std::string val)
  {
    setClpSimplexParam<std::string>(stringSimplexParams_,what,val) ;
  }
//@}

/*! \name ClpSolve parameter gets & sets

  These are parameters normally exposed through the ClpSolve interface. See
  \file ClpSolve.hpp for details.
*/
//@{
  /// Special options
  void setSpecialOption(int which, int value, int extrainfo = -1) ;
  int specialOption(int which) const ;
  /// Retrieve extra info
  int extraInfo(int which) const ;
  /// Solve type
  void setSolveType(int method, int extrainfo = -1) ;
  int solveType() const ;
  /// Presolve type
  void setPresolveType(int type, int passes = -1) ;
  int presolveType() const ;
  /// Presolve passes
  int presolvePasses() const ;
  /// Individual presolve transforms
  void setPresolveXform(std::string xform, bool onoff) ;
  bool presolveXform(std::string xform) const ;
  /// Longest column for substitution
  int substitution() const ;
  void setSubstitution(int val) ;
//@}

  inline int getAPIs(const char **&idents)
  { return (paramMgr_.getAPIs(idents)) ; }
  inline void *getAPIPtr(const char *ident)
  { return (paramMgr_.getAPIPtr(ident)) ; }

private:
  /*! \name Dynamic object management information */
  //@{
    /// Parameter management object
    ParamBEAPI_Imp<ClpSolveParamsAPI_ClpLite> paramMgr_ ;

    /// Clp dynamic library object
    DynamicLibrary *libClp_ ;
  //@}

  /*! \name Clp parameters from ClpSimplex

    Cached values for parameters normally handled by direct set/get
    in CplSimplex.
  */
  //@{

  template<class ValType> using KnownParams =
  std::map<std::string, ValType Osi2::ClpSolveParamsAPI_ClpLite::*> ;

  double primalZeroTol_ ;
  double dualZeroTol_ ;
  double dualObjLim_ ;
  double objOffset_ ;
  double objSense_ ;
  double infeasCost_ ;
  double smallElem_ ;
  double dualBound_ ;
  static KnownParams<double> doubleSimplexParams_ ;

  int iterCount_ ;
  int iterMax_ ;
  int scaling_ ;
  int perturbation_ ;
  int algorithm_ ;
  static KnownParams<int> integerSimplexParams_ ;

  std::string probName_ ;
  static KnownParams<std::string> stringSimplexParams_ ;

  /// Generic set
  template<class ValType> void setClpSimplexParam
    (KnownParams<ValType> knownMap,std::string param,ValType val) ;
  /// Generic get
  template<class RetType> RetType getClpSimplexParam
    (KnownParams<RetType> knownMap,std::string param) const ;
  //@}

  /// Our ClpSolve object
  Clp_Solve *clpSolve_ ;

  /// Presolve transforms
  typedef std::map<std::string,bool> PresolveXformMap ;
  PresolveXformMap xforms_ ;

} ;

}  // end namespace Osi2

#endif

