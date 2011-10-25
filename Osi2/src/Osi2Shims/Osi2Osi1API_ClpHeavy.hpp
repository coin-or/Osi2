/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
#ifndef Osi2Osi1API_Clp_HPP
#define Osi2Osi1API_Clp_HPP

#include "Osi2DynamicLibrary.hpp"

#include "Osi2API.hpp"
#include "Osi2Osi1API.hpp"

#include "OsiClpSolverInterface.hpp"

namespace Osi2 {

class Osi1API_ClpHeavy : public Osi1API, OsiClpSolverInterface {

public:

  /// Constructor
  Osi1API_ClpHeavy () ;

  /// Destructor
  ~Osi1API_ClpHeavy () ;

  inline void initialSolve() { OsiClpSolverInterface::initialSolve() ; }

  inline void resolve() { OsiClpSolverInterface::resolve() ; }


  inline bool setIntParam(OsiIntParam key, int value)
  { return (OsiClpSolverInterface::setIntParam(key,value)) ; }

  inline bool setDblParam(OsiDblParam key, double value)
  { return (OsiClpSolverInterface::setDblParam(key,value)) ; }

  inline bool setStrParam(OsiStrParam key, const std::string &value)
  { return (OsiClpSolverInterface::setStrParam(key,value)) ; }

  inline bool setHintParam(OsiHintParam key, bool yesNo = true,
  			   OsiHintStrength strength = OsiHintTry,
			   void *info = nullptr)
  { return (OsiClpSolverInterface::setHintParam(key,yesNo,strength,info)) ; }

  inline bool getIntParam(OsiIntParam key, int &value) const
  { return (OsiClpSolverInterface::getIntParam(key,value)) ; }

  inline bool getDblParam(OsiDblParam key, double &value) const
  { return (OsiClpSolverInterface::getDblParam(key,value)) ; }

  inline bool getStrParam(OsiStrParam key, std::string &value) const
  { return (OsiClpSolverInterface::getStrParam(key,value)) ; }

  inline bool getHintParam(OsiHintParam key, bool &yesNo,
  			   OsiHintStrength &strength, void *&info) const
  { return (OsiClpSolverInterface::getHintParam(key,yesNo,strength,info)) ; }

  inline bool getHintParam(OsiHintParam key, bool &yesNo,
  			   OsiHintStrength &strength) const
  { return (OsiClpSolverInterface::getHintParam(key,yesNo,strength)) ; }

  inline bool getHintParam(OsiHintParam key, bool &yesNo) const
  { return (OsiClpSolverInterface::getHintParam(key,yesNo)) ; }

  inline void copyParameters(Osi1API &rhs)
  { OsiSolverInterface &osi = dynamic_cast<OsiSolverInterface &>(rhs) ;
    OsiClpSolverInterface::copyParameters(osi) ; }


  inline double getIntegerTolerance() const
  { return (OsiClpSolverInterface::getIntegerTolerance()) ; }

  inline bool isAbandoned() const
  { return (OsiClpSolverInterface::isAbandoned()) ; }
  inline bool isProvenOptimal() const
  { return (OsiClpSolverInterface::isProvenOptimal()) ; }
  inline bool isProvenPrimalInfeasible() const
  { return (OsiClpSolverInterface::isProvenPrimalInfeasible()) ; }
  inline bool isProvenDualInfeasible() const
  { return (OsiClpSolverInterface::isProvenDualInfeasible()) ; }
  inline bool isPrimalObjectiveLimitReached() const
  { return (OsiClpSolverInterface::isPrimalObjectiveLimitReached()) ; }
  inline bool isIterationLimitReached() const
  { return (OsiClpSolverInterface::isIterationLimitReached()) ; }


  inline CoinWarmStart* getEmptyWarmStart() const
  { return (OsiClpSolverInterface::getEmptyWarmStart()) ; }

  inline CoinWarmStart* getWarmStart() const
  { return (OsiClpSolverInterface::getWarmStart()) ; }

  inline CoinWarmStart* getPointerToWarmStart(bool &mustDelete)
  { return (OsiClpSolverInterface::getPointerToWarmStart(mustDelete)) ; }

  inline bool setWarmStart(const CoinWarmStart *warmstart)
  { return (OsiClpSolverInterface::setWarmStart(warmstart)) ; }


  inline void markHotStart()
  { OsiClpSolverInterface::markHotStart() ; }

  inline void solveFromHotStart()
  { OsiClpSolverInterface::solveFromHotStart() ; }

  inline void unmarkHotStart()
  { OsiClpSolverInterface::unmarkHotStart() ; }


  inline int getNumCols() const
  { return (OsiClpSolverInterface::getNumCols()) ; }

  inline int getNumRows() const
  { return (OsiClpSolverInterface::getNumRows()) ; }

  inline int getNumElements() const
  { return (OsiClpSolverInterface::getNumElements()) ; }

  inline int getNumIntegers() const
  { return (OsiClpSolverInterface::getNumIntegers()) ; }

  inline const double* getColLower() const
  { return (OsiClpSolverInterface::getColLower()) ; }

  inline const double* getColUpper() const
  { return (OsiClpSolverInterface::getColUpper()) ; }

  inline const char* getRowSense() const
  { return (OsiClpSolverInterface::getRowSense()) ; }

  inline const double* getRightHandSide() const
  { return (OsiClpSolverInterface::getRightHandSide()) ; }

  inline const double* getRowRange() const
  { return (OsiClpSolverInterface::getRowRange()) ; }

  inline const double* getRowLower() const
  { return (OsiClpSolverInterface::getRowLower()) ; }

  inline const double* getRowUpper() const
  { return (OsiClpSolverInterface::getRowUpper()) ; }

  inline const double* getObjCoefficients() const
  { return (OsiClpSolverInterface::getObjCoefficients()) ; }

  inline double getObjSense() const
  { return (OsiClpSolverInterface::getObjSense()) ; }


  inline bool isContinuous(int ndx) const
  { return (OsiClpSolverInterface::isContinuous(ndx)) ; }

  inline bool isBinary(int ndx) const
  { return (OsiClpSolverInterface::isBinary(ndx)) ; }

  inline bool isInteger(int ndx) const
  { return (OsiClpSolverInterface::isInteger(ndx)) ; }

  inline bool isIntegerNonBinary(int ndx) const
  { return (OsiClpSolverInterface::isIntegerNonBinary(ndx)) ; }

  inline bool isFreeBinary(int ndx) const
  { return (OsiClpSolverInterface::isFreeBinary(ndx)) ; }

  inline const char* getColType(bool refresh) const
  { return (OsiClpSolverInterface::getColType(refresh)) ; }

  inline const CoinPackedMatrix* getMatrixByRow() const
  { return (OsiClpSolverInterface::getMatrixByRow()) ; }

  inline const CoinPackedMatrix* getMatrixByCol() const
  { return (OsiClpSolverInterface::getMatrixByCol()) ; }

  inline CoinPackedMatrix* getMutableMatrixByRow() const
  { return (OsiClpSolverInterface::getMutableMatrixByRow()) ; }

  inline CoinPackedMatrix* getMutableMatrixByCol() const
  { return (OsiClpSolverInterface::getMutableMatrixByCol()) ; }

  inline double getInfinity() const
  { return (OsiClpSolverInterface::getInfinity()) ; }

  inline const double* getColSolution() const
  { return (OsiClpSolverInterface::getColSolution()) ; }

  inline const double* getStrictColSolution()
  { return (OsiClpSolverInterface::getStrictColSolution()) ; }

  inline const double* getRowPrice() const
  { return (OsiClpSolverInterface::getRowPrice()) ; }

  inline const double* getReducedCost() const
  { return (OsiClpSolverInterface::getReducedCost()) ; }

  inline const double* getRowActivity() const
  { return (OsiClpSolverInterface::getRowActivity()) ; }

  inline double getObjValue() const
  { return (OsiClpSolverInterface::getObjValue()) ; }

  inline int getIterationCount() const
  { return (OsiClpSolverInterface::getIterationCount()) ; }

  inline std::vector<double*> getDualRays(int maxCnt, bool fullRay) const
  { return (OsiClpSolverInterface::getDualRays(maxCnt,fullRay)) ; }

  inline std::vector<double*> getPrimalRays(int maxCnt) const
  { return (OsiClpSolverInterface::getPrimalRays(maxCnt)) ; }

  inline OsiVectorInt getFractionalIndices(double tol) const
  { return (OsiClpSolverInterface::getFractionalIndices(tol)) ; }

  inline void setObjCoeff(int ndx, double val)
  { OsiClpSolverInterface::setObjCoeff(ndx,val) ; }

  inline void setObjCoeffSet(const int *first, const int *last,
  			     const double *valVec)
  { OsiClpSolverInterface::setObjCoeffSet(first,last,valVec) ; }

  inline void setObjective(const double *valVec)
  { OsiClpSolverInterface::setObjective(valVec) ; }

  inline void setObjSense(double sense)
  { OsiClpSolverInterface::setObjSense(sense) ; }

  inline void setColLower(int ndx, double lb)
  { OsiClpSolverInterface::setColLower(ndx,lb) ; }

  inline void setColLower(const double *lbVec)
  { OsiClpSolverInterface::setColLower(lbVec) ; }

  inline void setColUpper(int ndx, double ub)
  { OsiClpSolverInterface::setColUpper(ndx,ub) ; }

  inline void setColUpper(const double *ubVec)
  { OsiClpSolverInterface::setColUpper(ubVec) ; }

  inline void setColBounds(int ndx, double lb, double ub)
  { OsiClpSolverInterface::setColBounds(ndx,lb,ub) ; }

  inline void setColSetBounds(const int *first, const int *last,
  			      const double *bndVec)
  { OsiClpSolverInterface::setColSetBounds(first,last,bndVec) ; }

  inline void setRowLower(int ndx, double lb)
  { OsiClpSolverInterface::setRowLower(ndx,lb) ; }

  inline void setRowUpper(int ndx, double ub)
  { OsiClpSolverInterface::setRowUpper(ndx,ub) ; }

  inline void setRowBounds(int ndx, double lb, double ub)
  { OsiClpSolverInterface::setRowBounds(ndx,lb,ub) ; }

  inline void setRowSetBounds(const int *first, const int *last,
  			      const double *bndVec)
  { OsiClpSolverInterface::setRowSetBounds(first,last,bndVec) ; }

  inline void setRowType(int ndx, char sense, double rhs, double range)
  { OsiClpSolverInterface::setRowType(ndx,sense,rhs,range) ; }

  inline void setRowSetTypes(const int *first, const int *last,
  			     const char *senseVec,
			     const double *rhsVec, const double *rangeVec)
  { OsiClpSolverInterface::setRowSetTypes(first,last,
  					  senseVec,rhsVec,rangeVec) ; }

  inline void setColSolution(const double *valVec)
  { OsiClpSolverInterface::setColSolution(valVec) ; }

  inline void setRowPrice(const double *valVec)
  { OsiClpSolverInterface::setRowPrice(valVec) ; }

  inline int reducedCostFix(double gap, bool justInteger=true)
  { return (OsiClpSolverInterface::reducedCostFix(gap,justInteger)) ; }

  inline void setContinuous(int ndx)
  { OsiClpSolverInterface::setContinuous(ndx) ; }
  inline void setInteger(int ndx)
  { OsiClpSolverInterface::setInteger(ndx) ; }
  inline void setContinuous(const int *ndxVec, int len)
  { OsiClpSolverInterface::setContinuous(ndxVec,len) ; }
  inline void setInteger(const int *ndxVec, int len)
  { OsiClpSolverInterface::setInteger(ndxVec,len) ; }

  inline std::string dfltRowColName(char rc, int ndx,
  				    unsigned digits = 7) const
  { return (OsiClpSolverInterface::dfltRowColName(rc,ndx,digits)) ; }

  inline std::string getObjName(unsigned maxLen = static_cast<unsigned>(std::string::npos)) const
  { return (OsiClpSolverInterface::getObjName(maxLen)) ; }

  inline void setObjName(std::string name)
  { OsiClpSolverInterface::setObjName(name) ; }

  inline std::string getRowName(int ndx,
  				unsigned int maxLen = static_cast<unsigned>(std::string::npos)) const
  { return (OsiClpSolverInterface::getRowName(ndx,maxLen)) ; }

  /*
    \todo For occurrences of OsiNameVec, do I need to cast from
	  OsiSolverInterface::OsiNameVec <-> Osi1API::OsiNameVec?
  */
  inline const OsiSolverInterface::OsiNameVec &getRowNames()
  { return (OsiClpSolverInterface::getRowNames()) ; }

  inline void setRowName(int ndx, std::string name)
  { OsiClpSolverInterface::setRowName(ndx,name) ; }

  inline void setRowNames(OsiSolverInterface::OsiNameVec &names,
  			  int srcStart, int len, int tgtStart)
  { OsiClpSolverInterface::setRowNames(names,srcStart,len,tgtStart) ; }

  inline void deleteRowNames(int tgtStart, int len)
  { OsiClpSolverInterface::deleteRowNames(tgtStart,len) ; }

  inline std::string getColName(int ndx,
  				unsigned maxLen = static_cast<unsigned>(std::string::npos)) const
  { return (OsiClpSolverInterface::getColName(ndx,maxLen)) ; }

  inline const OsiSolverInterface::OsiNameVec &getColNames()
  { return (OsiClpSolverInterface::getColNames()) ; }

  inline void setColName(int ndx, std::string name)
  { OsiClpSolverInterface::setColName(ndx,name) ; }

  inline void setColNames(OsiSolverInterface::OsiNameVec &names,
  			  int srcStart, int len, int tgtStart)
  { OsiClpSolverInterface::setColNames(names,srcStart,len,tgtStart) ; }

  inline void deleteColNames(int tgtStart, int len)
  { OsiClpSolverInterface::deleteColNames(tgtStart,len) ; }

  inline void setRowColNames(const CoinMpsIO &mps)
  { OsiClpSolverInterface::setRowColNames(mps) ; }

  inline void setRowColNames(CoinModel &mod)
  { OsiClpSolverInterface::setRowColNames(mod) ; }

  inline void setRowColNames(CoinLpIO &mod)
  { OsiClpSolverInterface::setRowColNames(mod) ; }
  

  inline void addCol(const CoinPackedVectorBase &aj,
  		     double lj, double uj, double cj)
  { OsiClpSolverInterface::addCol(aj,lj,uj,cj) ; }

  inline void addCol(const CoinPackedVectorBase &aj,
  		     double lj, double uj, double cj, std::string name)
  { OsiClpSolverInterface::addCol(aj,lj,uj,cj,name) ; }

  inline void addCol(int len, const int *rowIndices,
  		     const double *aj, double lj, double uj, double cj)
  { OsiClpSolverInterface::addCol(len,rowIndices,aj,lj,uj,cj) ; }

  inline void addCol(int len, const int *rowIndices,
  		     const double *aj, double lj, double uj, double cj,
		     std::string name)
  { OsiClpSolverInterface::addCol(len,rowIndices,aj,lj,uj,cj,name) ; }

  inline void addCols(int numCols, const CoinPackedVectorBase *const *ajs,
  		      const double *ljs, const double *ujs, const double *cjs)
  { OsiClpSolverInterface::addCols(numCols,ajs,ljs,ujs,cjs) ; }

  inline void addCols(int numCols, const int *colStarts, const int *rowIndices,
  		      const double *aijs,
		      const double *ljs, const double *ujs, const double *cjs)
  { OsiClpSolverInterface::addCols(numCols,
  				   colStarts,rowIndices,aijs,ljs,ujs,cjs) ; }

  inline void addCols(const CoinBuild &mod)
  { OsiSolverInterface::addCols(mod) ; }

  inline int addCols(CoinModel &mod)
  { return (OsiSolverInterface::addCols(mod)) ; }

  inline void deleteCols(int num, const int *colIndices)
  { OsiClpSolverInterface::deleteCols(num,colIndices) ; }

  inline void addRow(const CoinPackedVectorBase &ai, double li, double ui)
  { OsiClpSolverInterface::addRow(ai,li,ui) ; }

  inline void addRow(const CoinPackedVectorBase &ai,
  		     double li, double ui, std::string name)
  { OsiClpSolverInterface::addRow(ai,li,ui,name) ; }

  inline void addRow(const CoinPackedVectorBase &ai,
  		     char sense, double li, double ui)
  { OsiClpSolverInterface::addRow(ai,sense,li,ui) ; }

  inline void addRow(const CoinPackedVectorBase &ai,
  		     char sense, double li, double ui, std::string name)
  { OsiClpSolverInterface::addRow(ai,sense,li,ui,name) ; }

  inline void addRow(int len, const int *colIndices,
  		     const double *ai, double li, double ui)
  { OsiClpSolverInterface::addRow(len,colIndices,ai,li,ui) ; }

  inline void addRows(int numRows, const CoinPackedVectorBase *const *ais,
  		      const double *lis, const double *uis)
  { OsiClpSolverInterface::addRows(numRows,ais,lis,uis) ; }

  inline void addRows(int numRows, const CoinPackedVectorBase *const *ais,
  		      const char *senses, const double *lis, const double *uis)
  { OsiClpSolverInterface::addRows(numRows,ais,senses,lis,uis) ; }

  inline void addRows(int numRows, const int *rowStarts, const int *colIndices,
  		      const double *aijs,
		      const double *lis, const double *uis)
  { OsiClpSolverInterface::addRows(numRows,
  				   rowStarts,colIndices,aijs,lis,uis) ; }

  inline void addRows(const CoinBuild &mod)
  { OsiSolverInterface::addRows(mod) ; }

  inline int addRows(CoinModel &mod)
  { return (OsiSolverInterface::addRows(mod)) ; }

  inline void deleteRows(int num, const int *rowIndices)
  { OsiClpSolverInterface::deleteRows(num,rowIndices) ; }

  inline void replaceMatrixOptional(const CoinPackedMatrix &mtx)
  { OsiClpSolverInterface::replaceMatrixOptional(mtx) ; }

  inline void replaceMatrix(const CoinPackedMatrix &mtx)
  { OsiClpSolverInterface::replaceMatrix(mtx) ; }

  inline void saveBaseModel()
  { OsiClpSolverInterface::saveBaseModel() ; }

  inline void restoreBaseModel(int numRows)
  { OsiClpSolverInterface::restoreBaseModel(numRows) ; }

/*
  This is going to be a problem. The declaration nets an
  `invalid covariant return type' error.

  inline Osi1API::ApplyCutsReturnCode applyCuts(const OsiCuts &cuts,
  						double eff = 0.0)
  { const OsiSolverInterface::ApplyCutsReturnCode &tmp =
  	OsiClpSolverInterface::applyCuts(cuts,eff) ;
    Osi1API::ApplyCutsReturnCode retval ;
    retval.intInconsistent_ = tmp.intInconsistent_ ;
    return (retval) ; }
*/

  inline void applyRowCuts(int numCuts, const OsiRowCut *cuts)
  { OsiClpSolverInterface::applyRowCuts(numCuts,cuts) ; }

  inline void applyRowCuts(int numCuts, const OsiRowCut **cuts)
  { OsiClpSolverInterface::applyRowCuts(numCuts,cuts) ; }

  inline void deleteBranchingInfo(int num, const int *colIndices)
  { OsiClpSolverInterface::deleteBranchingInfo(num,colIndices) ; }


  inline void loadProblem(const CoinPackedMatrix &mtx,
  			  const double *clbs, const double *cubs,
			  const double *obj,
			  const double *rlbs, const double *rubs)
  { loadProblem(mtx,clbs,cubs,obj,rlbs,rubs) ; }

  inline void assignProblem(CoinPackedMatrix *&mtx,
  			    double *&clbs, double *&cubs,
			    double *&obj, double *&rlbs, double *&rubs)
  { OsiClpSolverInterface::assignProblem(mtx,clbs,cubs,obj,rlbs,rubs) ; }

  inline void loadProblem(const CoinPackedMatrix &mtx,
  			  const double *clbs, const double *cubs,
			  const double *obj, const char *senses,
			  const double *rhss, const double *rngs)
  { OsiClpSolverInterface::loadProblem(mtx,clbs,cubs,obj,senses,rhss,rngs) ; }

  inline void assignProblem(CoinPackedMatrix *&mtx,
  			    double *&clbs, double *&cubs,
			    double *&obj, char *&senses,
			    double *&rhss, double *&rngs)
  { OsiClpSolverInterface::assignProblem(mtx,clbs,cubs,obj,senses,rhss,rngs) ; }

  inline void loadProblem(int numCols, int numRows,
  			  const CoinBigIndex *colStarts, const int *rowIndices,
			  const double *aijs,
			  const double *clbs, const double *cubs,
			  const double *obj,
			  const double *rlbs, const double *rubs)
  { OsiClpSolverInterface::loadProblem(numCols,numRows,
  				       colStarts,rowIndices,aijs,
				       clbs,cubs,obj,rlbs,rubs) ; }


  inline void loadProblem(int numCols, int numRows,
  			  const CoinBigIndex *colStarts, const int *rowIndices,
			  const double *aijs,
			  const double *clbs, const double *cubs,
			  const double *obj, const char *senses,
			  const double *rhss, const double *rngs)
  { OsiClpSolverInterface::loadProblem(numCols,numRows,
  				       colStarts,rowIndices,aijs,
				       clbs,cubs,obj,senses,rhss,rngs) ; }

  inline int loadFromCoinModel(CoinModel &mod, bool keepSolution = false)
  { return (OsiClpSolverInterface::loadFromCoinModel(mod,keepSolution)) ; }


  inline int readMps(const char *fname, const char *ext = "mps")
  { return (OsiClpSolverInterface::readMps(fname,ext)) ; }

  inline int readMps(const char *fname, const char *ext,
  		     int &numSets, CoinSet **&sets)
  { return (OsiClpSolverInterface::readMps(fname,ext,numSets,sets)) ; }

  inline int readGMPL(const char *fname, const char *dname = nullptr)
  { return (OsiClpSolverInterface::readGMPL(fname,dname)) ; }

  inline void writeMps(const char *fname, const char *ext = "mps",
  		       double objSense = 0.0) const
  { OsiClpSolverInterface::writeMps(fname,ext,objSense) ; }

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
  { OsiClpSolverInterface::writeLp(fname,ext,eps,numAcross,
  				   decimals,objSense,useRowNames) ; }

  inline void writeLp(FILE *fp,
  		      double eps = 1e-5, int numAcross = 10, int decimals = 5,
		      double objSense = 0.0, bool useRowNames = true) const
  { OsiClpSolverInterface::writeLp(fp,eps,numAcross,
  				   decimals,objSense,useRowNames) ; }

  inline int writeLpNative(const char *fname,
  			    char const* const* rowNames,
  			    char const* const* colNames,
			    double eps = 1e-5, int numAcross = 10,
			    int decimals = 5, double objSense = 0.0,
			    bool useRowNames = true) const
  { return (OsiClpSolverInterface::writeLpNative(fname,rowNames,colNames,
			      eps,numAcross,decimals,objSense,useRowNames)) ; }

  inline int writeLpNative(FILE *fp,
  			    char const* const* rowNames,
  			    char const* const* colNames,
			    double eps = 1e-5, int numAcross = 10,
			    int decimals = 5, double objSense = 0.0,
			    bool useRowNames = true) const
  { return (OsiClpSolverInterface::writeLpNative(fp,rowNames,colNames,
			      eps,numAcross,decimals,objSense,useRowNames)) ; }

  inline int readLp(const char *fname, double eps = 1e-5)
  { return (OsiSolverInterface::readLp(fname,eps)) ; }

  inline int readLp(FILE *fp, double eps = 1e-5)
  { return (OsiSolverInterface::readLp(fp,eps)) ; }


  inline void setApplicationData(void *info)
  { OsiClpSolverInterface::setApplicationData(info) ; }

  inline void setAuxiliaryInfo(OsiAuxInfo *info)
  { OsiClpSolverInterface::setAuxiliaryInfo(info) ; }

  inline void* getApplicationData() const
  { return (OsiClpSolverInterface::getApplicationData()) ; }

  inline OsiAuxInfo* getAuxiliaryInfo() const
  { return (OsiClpSolverInterface::getAuxiliaryInfo()) ; }


  inline void passInMessageHandler(CoinMessageHandler *hdl)
  { OsiClpSolverInterface::passInMessageHandler(hdl) ; }

  inline void setLanguage(CoinMessages::Language lang)
  { OsiClpSolverInterface::setLanguage(lang) ; }

  inline CoinMessageHandler* messageHandler() const
  { return (OsiClpSolverInterface::messageHandler()) ; }

  inline CoinMessages messages()
  { return (OsiClpSolverInterface::messages()) ; }

  inline CoinMessages* messagesPointer()
  { return (OsiClpSolverInterface::messagesPointer()) ; }

  inline bool defaultHandler() const
  { return (OsiClpSolverInterface::defaultHandler()) ; }


  inline void findIntegers(bool justCount)
  { OsiClpSolverInterface::findIntegers(justCount) ; }

  inline int findIntegersAndSOS(bool justCount)
  { return (OsiClpSolverInterface::findIntegersAndSOS(justCount)) ; }

  inline int numberObjects() const
  { return (OsiClpSolverInterface::numberObjects()) ; }

  inline void setNumberObjects(int num)
  { OsiClpSolverInterface::setNumberObjects(num) ; }

  inline OsiObject** objects() const
  { return (OsiClpSolverInterface::objects()) ; }

  inline const OsiObject* object(int ndx) const
  { return (OsiClpSolverInterface::object(ndx)) ; }

  inline OsiObject* modifiableObject(int ndx) const
  { return (OsiClpSolverInterface::modifiableObject(ndx)) ; }

  inline void deleteObjects()
  { OsiClpSolverInterface::deleteObjects() ; }

  inline void addObjects(int numObjs, OsiObject **objs)
  { OsiClpSolverInterface::addObjects(numObjs,objs) ; }

  inline double forceFeasible()
  { return (OsiClpSolverInterface::forceFeasible()) ; }


  inline void activateRowCutDebugger(const char *modelName)
  { OsiClpSolverInterface::activateRowCutDebugger(modelName) ; }

  inline void activateRowCutDebugger(const double *soln,
  				     bool enforceOptimality = true)
  { OsiClpSolverInterface::activateRowCutDebugger(soln,enforceOptimality) ; }

  inline const OsiRowCutDebugger* getRowCutDebugger() const
  { return (OsiClpSolverInterface::getRowCutDebugger()) ; }

  inline OsiRowCutDebugger* getRowCutDebuggerAlways() const
  { return (OsiClpSolverInterface::getRowCutDebuggerAlways()) ; }



  inline int canDoSimplexInterface() const
  { return (OsiClpSolverInterface::canDoSimplexInterface()) ; }

  inline void enableFactorization() const
  { OsiClpSolverInterface::enableFactorization() ; }

  inline void disableFactorization() const
  { OsiClpSolverInterface::disableFactorization() ; }

  inline bool basisIsAvailable() const
  { return (OsiClpSolverInterface::basisIsAvailable()) ; }

  inline void getBasisStatus(int *colStatus, int *rowStatus) const
  { OsiClpSolverInterface::getBasisStatus(colStatus,rowStatus) ; }

  inline int setBasisStatus(const int *colStatus, const int *rowStatus)
  { return (OsiClpSolverInterface::setBasisStatus(colStatus,rowStatus)) ; }

  inline void getReducedGradient(double *cbars, double *duals,
				 const double *obj) const
  { OsiClpSolverInterface::getReducedGradient(cbars,duals,obj) ; }

  inline void getBInvARow(int ndx, double *abari, double *betai = nullptr) const
  { OsiClpSolverInterface::getBInvARow(ndx,abari,betai) ; }

  inline void getBInvRow(int ndx, double *betai) const
  { OsiClpSolverInterface::getBInvRow(ndx,betai) ; }

  inline void getBInvACol(int ndx, double *abarj) const
  { OsiClpSolverInterface::getBInvACol(ndx,abarj) ; }

  inline void getBInvCol(int ndx, double *betaj) const
  { OsiClpSolverInterface::getBInvCol(ndx,betaj) ; }

  inline void getBasics(int *indices) const
  { OsiClpSolverInterface::getBasics(indices) ; }

  inline void enableSimplexInterface(bool doingPrimal)
  { OsiClpSolverInterface::enableSimplexInterface(doingPrimal) ; }

  inline void disableSimplexInterface()
  { OsiClpSolverInterface::disableSimplexInterface() ; }

  inline int pivot(int colIn, int colOut, int outStatus)
  { return (OsiClpSolverInterface::pivot(colIn,colOut,outStatus)) ; }

  inline int primalPivotResult(int colIn, int sign, int &colOut,
  			       int &outStatus, double &t, CoinPackedVector *dx)
  { return (OsiClpSolverInterface::primalPivotResult(colIn,sign,colOut,
  						     outStatus,t,dx)) ; }

  inline int dualPivotResult(int &colIn, int &sign, int colOut, int outStatus,
  			     double &t, CoinPackedVector *dx)
  { return (OsiClpSolverInterface::dualPivotResult(colIn,sign,colOut,
  						   outStatus,t,dx)) ; }

/*
  Another covariant return type error.

  inline Osi2::Osi1API* clone(bool copyData = true) const
  { return (OsiClpSolverInterface::clone(copyData)) ; }
*/

  inline void reset()
  { OsiClpSolverInterface::reset() ; }


} ;

}

#endif
