/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2Osi1API_GlpkHeavy.hpp
    \brief Declarations for GlpkHeavy implementation of the Osi2::Osi1API.
*/
#ifndef Osi2Osi1API_Glpk_HPP
#define Osi2Osi1API_Glpk_HPP

#include "Osi2DynamicLibrary.hpp"

#include "Osi2API.hpp"
#include "Osi2Osi1API.hpp"

#include "OsiGlpkSolverInterface.hpp"

namespace Osi2 {
/*! \brief Implementation class for Osi1API.

  Comments here relate to details of the implementation. Look at the
  documentation for Osi1API for general documentation.

  This class uses the multiple inheritance shim paradigm. LibOsiGlpk and other
  support libraries must be linked with the shim. Since the Osi2::Osi1API is
  intended to be an exact match for the original OsiSolverInterface definition,
  the implementation (with a few exceptions) consists of defining the virtual
  methods of Osi2::Osi1API as calls to the appropriate OsiGlpkSolverInterface
  methods.
*/
class Osi1API_GlpkHeavy : public Osi1API, public OsiGlpkSolverInterface {

public:

  /// \name Constructors and destructors
  //@{
  Osi1API_GlpkHeavy () ;

  Osi1API_GlpkHeavy (const Osi1API_GlpkHeavy &rhs) ;

  virtual ~Osi1API_GlpkHeavy () ;

  inline Osi1API_GlpkHeavy *clone(bool copyData = true) const
  { return (new Osi1API_GlpkHeavy(*this)) ; }

  inline void reset()
  { OsiGlpkSolverInterface::reset() ; }
  //@}


  /// \name Solve methods
  //@{
  inline void resolve() { OsiGlpkSolverInterface::resolve() ; }

  inline void initialSolve() { OsiGlpkSolverInterface::initialSolve() ; }
  //@}


  /// \name Parameter set/get methods
  //@{
  inline bool setIntParam(OsiIntParam key, int value)
  { return (OsiGlpkSolverInterface::setIntParam(key,value)) ; }

  inline bool setDblParam(OsiDblParam key, double value)
  { return (OsiGlpkSolverInterface::setDblParam(key,value)) ; }

  inline bool setStrParam(OsiStrParam key, const std::string &value)
  { return (OsiGlpkSolverInterface::setStrParam(key,value)) ; }

  inline bool setHintParam(OsiHintParam key, bool yesNo = true,
  			   OsiHintStrength strength = OsiHintTry,
			   void *info = nullptr)
  { return (OsiGlpkSolverInterface::setHintParam(key,yesNo,strength,info)) ; }

  inline bool getIntParam(OsiIntParam key, int &value) const
  { return (OsiGlpkSolverInterface::getIntParam(key,value)) ; }

  inline bool getDblParam(OsiDblParam key, double &value) const
  { return (OsiGlpkSolverInterface::getDblParam(key,value)) ; }

  inline bool getStrParam(OsiStrParam key, std::string &value) const
  { return (OsiGlpkSolverInterface::getStrParam(key,value)) ; }

  inline bool getHintParam(OsiHintParam key, bool &yesNo,
  			   OsiHintStrength &strength, void *&info) const
  { return (OsiGlpkSolverInterface::getHintParam(key,yesNo,strength,info)) ; }

  inline bool getHintParam(OsiHintParam key, bool &yesNo,
  			   OsiHintStrength &strength) const
  { return (OsiGlpkSolverInterface::getHintParam(key,yesNo,strength)) ; }

  inline bool getHintParam(OsiHintParam key, bool &yesNo) const
  { return (OsiGlpkSolverInterface::getHintParam(key,yesNo)) ; }

  inline void copyParameters(Osi1API &rhs)
  { OsiSolverInterface &osi = dynamic_cast<OsiSolverInterface &>(rhs) ;
    OsiGlpkSolverInterface::copyParameters(osi) ; }

  inline double getIntegerTolerance() const
  { return (OsiGlpkSolverInterface::getIntegerTolerance()) ; }
  //@}


  /// \name Methods returning info on how the solution process terminated
  //@{
  inline bool isAbandoned() const
  { return (OsiGlpkSolverInterface::isAbandoned()) ; }

  inline bool isProvenOptimal() const
  { return (OsiGlpkSolverInterface::isProvenOptimal()) ; }

  inline bool isProvenPrimalInfeasible() const
  { return (OsiGlpkSolverInterface::isProvenPrimalInfeasible()) ; }

  inline bool isProvenDualInfeasible() const
  { return (OsiGlpkSolverInterface::isProvenDualInfeasible()) ; }

  inline bool isPrimalObjectiveLimitReached() const
  { return (OsiGlpkSolverInterface::isPrimalObjectiveLimitReached()) ; }

  inline bool isDualObjectiveLimitReached() const
  { return (OsiGlpkSolverInterface::isDualObjectiveLimitReached()) ; }

  inline bool isIterationLimitReached() const
  { return (OsiGlpkSolverInterface::isIterationLimitReached()) ; }
  //@}


  /// \name Warm start methods
  //@{
  inline CoinWarmStart* getEmptyWarmStart() const
  { return (OsiGlpkSolverInterface::getEmptyWarmStart()) ; }

  inline CoinWarmStart* getWarmStart() const
  { return (OsiGlpkSolverInterface::getWarmStart()) ; }

  inline CoinWarmStart* getPointerToWarmStart(bool &mustDelete)
  { return (OsiGlpkSolverInterface::getPointerToWarmStart(mustDelete)) ; }

  inline bool setWarmStart(const CoinWarmStart *warmstart)
  { return (OsiGlpkSolverInterface::setWarmStart(warmstart)) ; }
  //@}


  /// \name Hot start methods
  //@{
  inline void markHotStart()
  { OsiGlpkSolverInterface::markHotStart() ; }

  inline void solveFromHotStart()
  { OsiGlpkSolverInterface::solveFromHotStart() ; }

  inline void unmarkHotStart()
  { OsiGlpkSolverInterface::unmarkHotStart() ; }

  inline int getNumCols() const
  { return (OsiGlpkSolverInterface::getNumCols()) ; }
  //@}


  /// \name Problem query methods
  //@{
  inline int getNumRows() const
  { return (OsiGlpkSolverInterface::getNumRows()) ; }

  inline int getNumElements() const
  { return (OsiGlpkSolverInterface::getNumElements()) ; }

  inline int getNumIntegers() const
  { return (OsiGlpkSolverInterface::getNumIntegers()) ; }

  inline const double* getColLower() const
  { return (OsiGlpkSolverInterface::getColLower()) ; }

  inline const double* getColUpper() const
  { return (OsiGlpkSolverInterface::getColUpper()) ; }

  inline const char* getRowSense() const
  { return (OsiGlpkSolverInterface::getRowSense()) ; }

  inline const double* getRightHandSide() const
  { return (OsiGlpkSolverInterface::getRightHandSide()) ; }

  inline const double* getRowRange() const
  { return (OsiGlpkSolverInterface::getRowRange()) ; }

  inline const double* getRowLower() const
  { return (OsiGlpkSolverInterface::getRowLower()) ; }

  inline const double* getRowUpper() const
  { return (OsiGlpkSolverInterface::getRowUpper()) ; }

  inline const double* getObjCoefficients() const
  { return (OsiGlpkSolverInterface::getObjCoefficients()) ; }

  inline double getObjSense() const
  { return (OsiGlpkSolverInterface::getObjSense()) ; }
  //@}


  /// \name Solution query methods
  //@{
  inline bool isContinuous(int ndx) const
  { return (OsiGlpkSolverInterface::isContinuous(ndx)) ; }

  inline bool isBinary(int ndx) const
  { return (OsiGlpkSolverInterface::isBinary(ndx)) ; }

  inline bool isInteger(int ndx) const
  { return (OsiGlpkSolverInterface::isInteger(ndx)) ; }

  inline bool isIntegerNonBinary(int ndx) const
  { return (OsiGlpkSolverInterface::isIntegerNonBinary(ndx)) ; }

  inline bool isFreeBinary(int ndx) const
  { return (OsiGlpkSolverInterface::isFreeBinary(ndx)) ; }

  inline const char* getColType(bool refresh) const
  { return (OsiGlpkSolverInterface::getColType(refresh)) ; }

  inline const CoinPackedMatrix* getMatrixByRow() const
  { return (OsiGlpkSolverInterface::getMatrixByRow()) ; }

  inline const CoinPackedMatrix* getMatrixByCol() const
  { return (OsiGlpkSolverInterface::getMatrixByCol()) ; }

  inline CoinPackedMatrix* getMutableMatrixByRow() const
  { return (OsiGlpkSolverInterface::getMutableMatrixByRow()) ; }

  inline CoinPackedMatrix* getMutableMatrixByCol() const
  { return (OsiGlpkSolverInterface::getMutableMatrixByCol()) ; }

  inline double getInfinity() const
  { return (OsiGlpkSolverInterface::getInfinity()) ; }
  //@}


  /// \name Methods to modify the objective, bounds, and solution
  //@{
  inline const double* getColSolution() const
  { return (OsiGlpkSolverInterface::getColSolution()) ; }

  inline const double* getStrictColSolution()
  { return (OsiGlpkSolverInterface::getStrictColSolution()) ; }

  inline const double* getRowPrice() const
  { return (OsiGlpkSolverInterface::getRowPrice()) ; }

  inline const double* getReducedCost() const
  { return (OsiGlpkSolverInterface::getReducedCost()) ; }

  inline const double* getRowActivity() const
  { return (OsiGlpkSolverInterface::getRowActivity()) ; }

  inline double getObjValue() const
  { return (OsiGlpkSolverInterface::getObjValue()) ; }

  inline int getIterationCount() const
  { return (OsiGlpkSolverInterface::getIterationCount()) ; }

  inline std::vector<double*> getDualRays(int maxCnt, bool fullRay) const
  { return (OsiGlpkSolverInterface::getDualRays(maxCnt,fullRay)) ; }

  inline std::vector<double*> getPrimalRays(int maxCnt) const
  { return (OsiGlpkSolverInterface::getPrimalRays(maxCnt)) ; }

  inline OsiVectorInt getFractionalIndices(double tol) const
  { return (OsiGlpkSolverInterface::getFractionalIndices(tol)) ; }
  //@}


  /// \name Methods to modify the objective, bounds, and solution
  //@{
  inline void setObjCoeff(int ndx, double val)
  { OsiGlpkSolverInterface::setObjCoeff(ndx,val) ; }

  inline void setObjCoeffSet(const int *first, const int *last,
  			     const double *valVec)
  { OsiGlpkSolverInterface::setObjCoeffSet(first,last,valVec) ; }

  inline void setObjective(const double *valVec)
  { OsiGlpkSolverInterface::setObjective(valVec) ; }

  inline void setObjSense(double sense)
  { OsiGlpkSolverInterface::setObjSense(sense) ; }

  inline void setColLower(int ndx, double lb)
  { OsiGlpkSolverInterface::setColLower(ndx,lb) ; }

  inline void setColLower(const double *lbVec)
  { OsiGlpkSolverInterface::setColLower(lbVec) ; }

  inline void setColUpper(int ndx, double ub)
  { OsiGlpkSolverInterface::setColUpper(ndx,ub) ; }

  inline void setColUpper(const double *ubVec)
  { OsiGlpkSolverInterface::setColUpper(ubVec) ; }

  inline void setColBounds(int ndx, double lb, double ub)
  { OsiGlpkSolverInterface::setColBounds(ndx,lb,ub) ; }

  inline void setColSetBounds(const int *first, const int *last,
  			      const double *bndVec)
  { OsiGlpkSolverInterface::setColSetBounds(first,last,bndVec) ; }

  inline void setRowLower(int ndx, double lb)
  { OsiGlpkSolverInterface::setRowLower(ndx,lb) ; }

  inline void setRowUpper(int ndx, double ub)
  { OsiGlpkSolverInterface::setRowUpper(ndx,ub) ; }

  inline void setRowBounds(int ndx, double lb, double ub)
  { OsiGlpkSolverInterface::setRowBounds(ndx,lb,ub) ; }

  inline void setRowSetBounds(const int *first, const int *last,
  			      const double *bndVec)
  { OsiGlpkSolverInterface::setRowSetBounds(first,last,bndVec) ; }

  inline void setRowType(int ndx, char sense, double rhs, double range)
  { OsiGlpkSolverInterface::setRowType(ndx,sense,rhs,range) ; }

  inline void setRowSetTypes(const int *first, const int *last,
  			     const char *senseVec,
			     const double *rhsVec, const double *rangeVec)
  { OsiGlpkSolverInterface::setRowSetTypes(first,last,
  					  senseVec,rhsVec,rangeVec) ; }

  inline void setColSolution(const double *valVec)
  { OsiGlpkSolverInterface::setColSolution(valVec) ; }

  inline void setRowPrice(const double *valVec)
  { OsiGlpkSolverInterface::setRowPrice(valVec) ; }

  inline int reducedCostFix(double gap, bool justInteger=true)
  { return (OsiGlpkSolverInterface::reducedCostFix(gap,justInteger)) ; }
  //@}


  /// \name Methods to set variable type
  //@{
  inline void setContinuous(int ndx)
  { OsiGlpkSolverInterface::setContinuous(ndx) ; }
  inline void setInteger(int ndx)
  { OsiGlpkSolverInterface::setInteger(ndx) ; }
  inline void setContinuous(const int *ndxVec, int len)
  { OsiGlpkSolverInterface::setContinuous(ndxVec,len) ; }
  inline void setInteger(const int *ndxVec, int len)
  { OsiGlpkSolverInterface::setInteger(ndxVec,len) ; }
  //@}


  /// \name Methods for row and column names
  //@{
  inline std::string dfltRowColName(char rc, int ndx,
  				    unsigned digits = 7) const
  { return (OsiGlpkSolverInterface::dfltRowColName(rc,ndx,digits)) ; }

  inline std::string getObjName(unsigned maxLen = static_cast<unsigned>(std::string::npos)) const
  { return (OsiGlpkSolverInterface::getObjName(maxLen)) ; }

  inline void setObjName(std::string name)
  { OsiGlpkSolverInterface::setObjName(name) ; }

  inline std::string getRowName(int ndx,
  				unsigned int maxLen = static_cast<unsigned>(std::string::npos)) const
  { return (OsiGlpkSolverInterface::getRowName(ndx,maxLen)) ; }

  /*
    \todo For occurrences of OsiNameVec, do I need to cast from
	  OsiSolverInterface::OsiNameVec <-> Osi1API::OsiNameVec?
  */
  inline const OsiSolverInterface::OsiNameVec &getRowNames()
  { return (OsiGlpkSolverInterface::getRowNames()) ; }

  inline void setRowName(int ndx, std::string name)
  { OsiGlpkSolverInterface::setRowName(ndx,name) ; }

  inline void setRowNames(OsiSolverInterface::OsiNameVec &names,
  			  int srcStart, int len, int tgtStart)
  { OsiGlpkSolverInterface::setRowNames(names,srcStart,len,tgtStart) ; }

  inline void deleteRowNames(int tgtStart, int len)
  { OsiGlpkSolverInterface::deleteRowNames(tgtStart,len) ; }

  inline std::string getColName(int ndx,
  				unsigned maxLen = static_cast<unsigned>(std::string::npos)) const
  { return (OsiGlpkSolverInterface::getColName(ndx,maxLen)) ; }

  inline const OsiSolverInterface::OsiNameVec &getColNames()
  { return (OsiGlpkSolverInterface::getColNames()) ; }

  inline void setColName(int ndx, std::string name)
  { OsiGlpkSolverInterface::setColName(ndx,name) ; }

  inline void setColNames(OsiSolverInterface::OsiNameVec &names,
  			  int srcStart, int len, int tgtStart)
  { OsiGlpkSolverInterface::setColNames(names,srcStart,len,tgtStart) ; }

  inline void deleteColNames(int tgtStart, int len)
  { OsiGlpkSolverInterface::deleteColNames(tgtStart,len) ; }

  inline void setRowColNames(const CoinMpsIO &mps)
  { OsiGlpkSolverInterface::setRowColNames(mps) ; }

  inline void setRowColNames(CoinModel &mod)
  { OsiGlpkSolverInterface::setRowColNames(mod) ; }

  inline void setRowColNames(CoinLpIO &mod)
  { OsiGlpkSolverInterface::setRowColNames(mod) ; }
  //@}


  /// \name Methods to modify the constraint system
  //@{

  inline void addCol(const CoinPackedVectorBase &aj,
  		     double lj, double uj, double cj)
  { OsiGlpkSolverInterface::addCol(aj,lj,uj,cj) ; }

  inline void addCol(const CoinPackedVectorBase &aj,
  		     double lj, double uj, double cj, std::string name)
  { OsiGlpkSolverInterface::addCol(aj,lj,uj,cj,name) ; }

  inline void addCol(int len, const int *rowIndices,
  		     const double *aj, double lj, double uj, double cj)
  { OsiGlpkSolverInterface::addCol(len,rowIndices,aj,lj,uj,cj) ; }

  inline void addCol(int len, const int *rowIndices,
  		     const double *aj, double lj, double uj, double cj,
		     std::string name)
  { OsiGlpkSolverInterface::addCol(len,rowIndices,aj,lj,uj,cj,name) ; }

  inline void addCols(int numCols, const CoinPackedVectorBase *const *ajs,
  		      const double *ljs, const double *ujs, const double *cjs)
  { OsiGlpkSolverInterface::addCols(numCols,ajs,ljs,ujs,cjs) ; }

  inline void addCols(int numCols, const int *colStarts, const int *rowIndices,
  		      const double *aijs,
		      const double *ljs, const double *ujs, const double *cjs)
  { OsiGlpkSolverInterface::addCols(numCols,
  				   colStarts,rowIndices,aijs,ljs,ujs,cjs) ; }

  inline void addCols(const CoinBuild &mod)
  { OsiSolverInterface::addCols(mod) ; }

  inline int addCols(CoinModel &mod)
  { return (OsiSolverInterface::addCols(mod)) ; }

  inline void deleteCols(int num, const int *colIndices)
  { OsiGlpkSolverInterface::deleteCols(num,colIndices) ; }

  inline void addRow(const CoinPackedVectorBase &ai, double li, double ui)
  { OsiGlpkSolverInterface::addRow(ai,li,ui) ; }

  inline void addRow(const CoinPackedVectorBase &ai,
  		     double li, double ui, std::string name)
  { OsiGlpkSolverInterface::addRow(ai,li,ui,name) ; }

  inline void addRow(const CoinPackedVectorBase &ai,
  		     char sense, double li, double ui)
  { OsiGlpkSolverInterface::addRow(ai,sense,li,ui) ; }

  inline void addRow(const CoinPackedVectorBase &ai,
  		     char sense, double li, double ui, std::string name)
  { OsiGlpkSolverInterface::addRow(ai,sense,li,ui,name) ; }

  inline void addRow(int len, const int *colIndices,
  		     const double *ai, double li, double ui)
  { OsiGlpkSolverInterface::addRow(len,colIndices,ai,li,ui) ; }

  inline void addRows(int numRows, const CoinPackedVectorBase *const *ais,
  		      const double *lis, const double *uis)
  { OsiGlpkSolverInterface::addRows(numRows,ais,lis,uis) ; }

  inline void addRows(int numRows, const CoinPackedVectorBase *const *ais,
  		      const char *senses, const double *lis, const double *uis)
  { OsiGlpkSolverInterface::addRows(numRows,ais,senses,lis,uis) ; }

  inline void addRows(int numRows, const int *rowStarts, const int *colIndices,
  		      const double *aijs,
		      const double *lis, const double *uis)
  { OsiGlpkSolverInterface::addRows(numRows,
  				   rowStarts,colIndices,aijs,lis,uis) ; }

  inline void addRows(const CoinBuild &mod)
  { OsiSolverInterface::addRows(mod) ; }

  inline int addRows(CoinModel &mod)
  { return (OsiSolverInterface::addRows(mod)) ; }

  inline void deleteRows(int num, const int *rowIndices)
  { OsiGlpkSolverInterface::deleteRows(num,rowIndices) ; }

  inline void replaceMatrixOptional(const CoinPackedMatrix &mtx)
  { OsiGlpkSolverInterface::replaceMatrixOptional(mtx) ; }

  inline void replaceMatrix(const CoinPackedMatrix &mtx)
  { OsiGlpkSolverInterface::replaceMatrix(mtx) ; }

  inline void saveBaseModel()
  { OsiGlpkSolverInterface::saveBaseModel() ; }

  inline void restoreBaseModel(int numRows)
  { OsiGlpkSolverInterface::restoreBaseModel(numRows) ; }

/*! \brief Bridge between OsiSolverInterface::applyCuts and
	   Osi1API::applyCuts.
  
  Covariant return works only for pointers and references. Unfortunately,
  OsiSolverInterface::applyCuts returns the full object, so there's no way to
  override applyCuts and finesse a covariant return value. Hence this
  workaround, which constructs an Osi1API_GlpkHeavy::ApplyCutsReturnCode
  object from the OsiSolverInterface::ApplyCutsReturnCode object and returns
  it.
*/
  inline Osi1API::ApplyCutsReturnCode applyCutsPrivate(const OsiCuts &cuts,
  						double eff = 0.0)
  { 
    const OsiSolverInterface::ApplyCutsReturnCode &tmp =
  	OsiGlpkSolverInterface::applyCuts(cuts,eff) ;
    const Osi1API::ApplyCutsReturnCode
        retval(tmp.getNumInconsistent(),
	       tmp.getNumInconsistentWrtIntegerModel(),
	       tmp.getNumInfeasible(),
	       tmp.getNumIneffective(),
	       tmp.getNumApplied()) ;
    return (retval) ; }

  inline void applyRowCuts(int numCuts, const OsiRowCut *cuts)
  { OsiGlpkSolverInterface::applyRowCuts(numCuts,cuts) ; }

  inline void applyRowCuts(int numCuts, const OsiRowCut **cuts)
  { OsiGlpkSolverInterface::applyRowCuts(numCuts,cuts) ; }

  inline void deleteBranchingInfo(int num, const int *colIndices)
  { OsiGlpkSolverInterface::deleteBranchingInfo(num,colIndices) ; }
  //@}


  /// \name Methods for problem input and output
  //@{
  inline void loadProblem(const CoinPackedMatrix &mtx,
  			  const double *clbs, const double *cubs,
			  const double *obj,
			  const double *rlbs, const double *rubs)
  { OsiGlpkSolverInterface::loadProblem(mtx,clbs,cubs,obj,rlbs,rubs) ; }

  inline void assignProblem(CoinPackedMatrix *&mtx,
  			    double *&clbs, double *&cubs,
			    double *&obj, double *&rlbs, double *&rubs)
  { OsiGlpkSolverInterface::assignProblem(mtx,clbs,cubs,obj,rlbs,rubs) ; }

  inline void loadProblem(const CoinPackedMatrix &mtx,
  			  const double *clbs, const double *cubs,
			  const double *obj, const char *senses,
			  const double *rhss, const double *rngs)
  { OsiGlpkSolverInterface::loadProblem(mtx,clbs,cubs,obj,senses,rhss,rngs) ; }

  inline void assignProblem(CoinPackedMatrix *&mtx,
  			    double *&clbs, double *&cubs,
			    double *&obj, char *&senses,
			    double *&rhss, double *&rngs)
  { OsiGlpkSolverInterface::assignProblem(mtx,clbs,cubs,obj,senses,rhss,rngs) ; }

  inline void loadProblem(int numCols, int numRows,
  			  const CoinBigIndex *colStarts, const int *rowIndices,
			  const double *aijs,
			  const double *clbs, const double *cubs,
			  const double *obj,
			  const double *rlbs, const double *rubs)
  { OsiGlpkSolverInterface::loadProblem(numCols,numRows,
  				       colStarts,rowIndices,aijs,
				       clbs,cubs,obj,rlbs,rubs) ; }


  inline void loadProblem(int numCols, int numRows,
  			  const CoinBigIndex *colStarts, const int *rowIndices,
			  const double *aijs,
			  const double *clbs, const double *cubs,
			  const double *obj, const char *senses,
			  const double *rhss, const double *rngs)
  { OsiGlpkSolverInterface::loadProblem(numCols,numRows,
  				       colStarts,rowIndices,aijs,
				       clbs,cubs,obj,senses,rhss,rngs) ; }

  inline int loadFromCoinModel(CoinModel &mod, bool keepSolution = false)
  { return (OsiGlpkSolverInterface::loadFromCoinModel(mod,keepSolution)) ; }


  inline int readMps(const char *fname, const char *ext = "mps")
  { return (OsiGlpkSolverInterface::readMps(fname,ext)) ; }

  inline int readMps(const char *fname, const char *ext,
  		     int &numSets, CoinSet **&sets)
  { return (OsiGlpkSolverInterface::readMps(fname,ext,numSets,sets)) ; }

  inline int readGMPL(const char *fname, const char *dname = nullptr)
  { return (OsiGlpkSolverInterface::readGMPL(fname,dname)) ; }

  inline void writeMps(const char *fname, const char *ext = "mps",
  		       double objSense = 0.0) const
  { OsiGlpkSolverInterface::writeMps(fname,ext,objSense) ; }

  inline int writeMpsNative(const char *fname,
  			    const char **rowNames, const char **colNames,
			    int fmtType = 0, int numAcross = 2,
			    double objSense = 0.0, int numSOS = 0,
			    const CoinSet *setInfo = nullptr) const
  { return (OsiSolverInterface::writeMpsNative(fname,rowNames,colNames,
			      fmtType,numAcross,objSense,numSOS,setInfo)) ; }

  inline void writeLp(const char *fname, const char *ext = "lp",
  		      double eps = 1e-5, int numAcross = 10, int decimals = 5,
		      double objSense = 0.0, bool useRowNames = true) const
  { OsiGlpkSolverInterface::writeLp(fname,ext,eps,numAcross,
  				   decimals,objSense,useRowNames) ; }

  inline void writeLp(FILE *fp,
  		      double eps = 1e-5, int numAcross = 10, int decimals = 5,
		      double objSense = 0.0, bool useRowNames = true) const
  { OsiGlpkSolverInterface::writeLp(fp,eps,numAcross,
  				   decimals,objSense,useRowNames) ; }

  inline int writeLpNative(const char *fname,
  			    char const* const* rowNames,
  			    char const* const* colNames,
			    double eps = 1e-5, int numAcross = 10,
			    int decimals = 5, double objSense = 0.0,
			    bool useRowNames = true) const
  { return (OsiGlpkSolverInterface::writeLpNative(fname,rowNames,colNames,
			      eps,numAcross,decimals,objSense,useRowNames)) ; }

  inline int writeLpNative(FILE *fp,
  			    char const* const* rowNames,
  			    char const* const* colNames,
			    double eps = 1e-5, int numAcross = 10,
			    int decimals = 5, double objSense = 0.0,
			    bool useRowNames = true) const
  { return (OsiGlpkSolverInterface::writeLpNative(fp,rowNames,colNames,
			      eps,numAcross,decimals,objSense,useRowNames)) ; }

  inline int readLp(const char *fname, double eps = 1e-5)
  { return (OsiSolverInterface::readLp(fname,eps)) ; }

  inline int readLp(FILE *fp, double eps = 1e-5)
  { return (OsiSolverInterface::readLp(fp,eps)) ; }
  //@}


  /// \name Setting/Accessing application data
  //@{
  inline void setApplicationData(void *info)
  { OsiGlpkSolverInterface::setApplicationData(info) ; }

  inline void setAuxiliaryInfo(OsiAuxInfo *info)
  { OsiGlpkSolverInterface::setAuxiliaryInfo(info) ; }

  inline void* getApplicationData() const
  { return (OsiGlpkSolverInterface::getApplicationData()) ; }

  inline OsiAuxInfo* getAuxiliaryInfo() const
  { return (OsiGlpkSolverInterface::getAuxiliaryInfo()) ; }
  //@}


  /// \name Message handling
  //@{
  inline void passInMessageHandler(CoinMessageHandler *hdl)
  { OsiGlpkSolverInterface::passInMessageHandler(hdl) ; }

  inline void setLanguage(CoinMessages::Language lang)
  { OsiGlpkSolverInterface::setLanguage(lang) ; }

  inline CoinMessageHandler* messageHandler() const
  { return (OsiGlpkSolverInterface::messageHandler()) ; }

  inline CoinMessages messages()
  { return (OsiGlpkSolverInterface::messages()) ; }

  inline CoinMessages* messagesPointer()
  { return (OsiGlpkSolverInterface::messagesPointer()) ; }

  inline bool defaultHandler() const
  { return (OsiGlpkSolverInterface::defaultHandler()) ; }
  //@}


  /// \name Methods for dealing with discontinuities other than integers
  //@{
  inline void findIntegers(bool justCount)
  { OsiGlpkSolverInterface::findIntegers(justCount) ; }

  inline int findIntegersAndSOS(bool justCount)
  { return (OsiGlpkSolverInterface::findIntegersAndSOS(justCount)) ; }

  inline int numberObjects() const
  { return (OsiGlpkSolverInterface::numberObjects()) ; }

  inline void setNumberObjects(int num)
  { OsiGlpkSolverInterface::setNumberObjects(num) ; }

  inline OsiObject** objects() const
  { return (OsiGlpkSolverInterface::objects()) ; }

  inline const OsiObject* object(int ndx) const
  { return (OsiGlpkSolverInterface::object(ndx)) ; }

  inline OsiObject* modifiableObject(int ndx) const
  { return (OsiGlpkSolverInterface::modifiableObject(ndx)) ; }

  inline void deleteObjects()
  { OsiGlpkSolverInterface::deleteObjects() ; }

  inline void addObjects(int numObjs, OsiObject **objs)
  { OsiGlpkSolverInterface::addObjects(numObjs,objs) ; }

  inline double forceFeasible()
  { return (OsiGlpkSolverInterface::forceFeasible()) ; }
  //@}


  /// \name Methods related to testing generated cuts
  //@{
  inline void activateRowCutDebugger(const char *modelName)
  { OsiGlpkSolverInterface::activateRowCutDebugger(modelName) ; }

  inline void activateRowCutDebugger(const double *soln,
  				     bool enforceOptimality = true)
  { OsiGlpkSolverInterface::activateRowCutDebugger(soln,enforceOptimality) ; }

  inline const OsiRowCutDebugger* getRowCutDebugger() const
  { return (OsiGlpkSolverInterface::getRowCutDebugger()) ; }

  inline OsiRowCutDebugger* getRowCutDebuggerAlways() const
  { return (OsiGlpkSolverInterface::getRowCutDebuggerAlways()) ; }
  //@}


  /// \name OSI Simplex Interface
  //@{
  inline int canDoSimplexInterface() const
  { return (OsiGlpkSolverInterface::canDoSimplexInterface()) ; }
  //@}


  /// \name OSI Simplex Interface Group I
  //@{
  inline void enableFactorization() const
  { OsiGlpkSolverInterface::enableFactorization() ; }

  inline void disableFactorization() const
  { OsiGlpkSolverInterface::disableFactorization() ; }

  inline bool basisIsAvailable() const
  { return (OsiGlpkSolverInterface::basisIsAvailable()) ; }

  inline void getBasisStatus(int *colStatus, int *rowStatus) const
  { OsiGlpkSolverInterface::getBasisStatus(colStatus,rowStatus) ; }

  inline int setBasisStatus(const int *colStatus, const int *rowStatus)
  { return (OsiGlpkSolverInterface::setBasisStatus(colStatus,rowStatus)) ; }

  inline void getReducedGradient(double *cbars, double *duals,
				 const double *obj) const
  { OsiGlpkSolverInterface::getReducedGradient(cbars,duals,obj) ; }

  inline void getBInvARow(int ndx, double *abari, double *betai = nullptr) const
  { OsiGlpkSolverInterface::getBInvARow(ndx,abari,betai) ; }

  inline void getBInvRow(int ndx, double *betai) const
  { OsiGlpkSolverInterface::getBInvRow(ndx,betai) ; }

  inline void getBInvACol(int ndx, double *abarj) const
  { OsiGlpkSolverInterface::getBInvACol(ndx,abarj) ; }

  inline void getBInvCol(int ndx, double *betaj) const
  { OsiGlpkSolverInterface::getBInvCol(ndx,betaj) ; }

  inline void getBasics(int *indices) const
  { OsiGlpkSolverInterface::getBasics(indices) ; }
  //@}


  /// \name OSI Simplex Interface Group II
  //@{
  inline void enableSimplexInterface(bool doingPrimal)
  { OsiGlpkSolverInterface::enableSimplexInterface(doingPrimal) ; }

  inline void disableSimplexInterface()
  { OsiGlpkSolverInterface::disableSimplexInterface() ; }

  inline int pivot(int colIn, int colOut, int outStatus)
  { return (OsiGlpkSolverInterface::pivot(colIn,colOut,outStatus)) ; }

  inline int primalPivotResult(int colIn, int sign, int &colOut,
  			       int &outStatus, double &t, CoinPackedVector *dx)
  { return (OsiGlpkSolverInterface::primalPivotResult(colIn,sign,colOut,
  						     outStatus,t,dx)) ; }

  inline int dualPivotResult(int &colIn, int &sign, int colOut, int outStatus,
  			     double &t, CoinPackedVector *dx)
  { return (OsiGlpkSolverInterface::dualPivotResult(colIn,sign,colOut,
  						   outStatus,t,dx)) ; }
  //@}

} ;

}    // end namespace Osi2

#endif
