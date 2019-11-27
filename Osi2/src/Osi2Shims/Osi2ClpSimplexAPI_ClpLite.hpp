/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ClpSimplexAPI_ClpLite.hpp
    \brief The clp `lite' implementation of Osi2::ClpSimplexAPI
*/
#ifndef Osi2ClpSimplexAPI_ClpLite_HPP
#define Osi2ClpSimplexAPI_ClpLite_HPP

#include "Osi2API.hpp"
#define COIN_EXTERN_C
#include "Osi2ClpSimplexAPI.hpp"

#include "Osi2ParamBEAPI_Imp.hpp"

namespace Osi2 {

class DynamicLibrary ;

/*! \brief Proof of concept API.

  Implementation of the ClpSimplexAPI. Which see for user documentation of the
  methods implemented here.
*/
class ClpSimplexAPI_ClpLite : public ClpSimplexAPI {

public:
/*! \name Constructors, destructor, and copy */
//@{
  /// Constructor
  ClpSimplexAPI_ClpLite(DynamicLibrary *libClp) ;

  /// Destructor
  ~ClpSimplexAPI_ClpLite() ;
//@}

/*! \name Methods to load, modify, and retrieve a problem. */
//@{
  /// Load a problem (column major order).
  void loadProblem(const int numcols, const int numrows,
      const CoinBigIndex *start, const int *index, const double *value,
      const double *collb, const double *colub,
      const double *obj,
      const double *rowlb, const double *rowub) ;

  /// Load a quadratic objective (matrix portion).
  void loadQuadraticObjective (const int numcols,
      const CoinBigIndex *start, const int *index, const double *value) ;

  /// Get integer information.
  char *integerInformation() const ;
  /// Load integer information.
  void copyInIntegerInformation(const char *info) ;
  /// Delete integer information.
  void deleteIntegerInformation() ;

  /// Resize rim part of model.
  void resize(int newNumberRows, int newNumberColumns) ;

  /// Get number of rows.
  int numberRows() ;
  /// Add rows.
  void addRows(int number, const double *rowLower, const double *rowUpper,
      const CoinBigIndex *rowStart, const int *index, const double *value) ;
  /// Delete rows.
  void deleteRows(int number, const int *which) ;

  /// Get number of columns.
  int numberColumns() ;
  /// Add columns.
  void addColumns(int number,
      const double *colLower, const double *colUpper, const double *objective,
      const CoinBigIndex *colStarts, const int *index, const double *value) ;
  /// Delete columns.
  void deleteColumns(int number, const int *which) ;

  /// Get row lower bounds.
  double *rowLower() const ;
  /// Change row lower bounds.
  void chgRowLower(const double *value) ;

  /// Get row upper bounds.
  double *rowUpper() const ;
  /// Change row upper bounds.
  void chgRowUpper(const double *value) ;

  /// Get column lower bounds.
  double *columnLower() const ;
  /// Change column lower bounds.
  void chgColumnLower(const double *value) ;

  /// Get column upper bounds.
  double *columnUpper() const ;
  /// Change column upper bounds.
  void chgColumnUpper(const double *value) ;

  /// Get objective coefficients.
  double *objective() const ;
  /// Change cobjective coefficients.
  void chgObjCoefficients(const double *value) ;

  /// Get the number of non-zero entries in the matrix.
  CoinBigIndex getNumElements() const ;
  /// Get the column start vector for the matrix.
  const CoinBigIndex *getVectorStarts() const ;
  /// Get the column length vector for the matrix.
  const int *getVectorLengths() const ;
  /// Get the row index vector for the matrix.
  const int *getIndices() const ;
  /// Get the element values in the matrix.
  const double *getElements() const ;
  /// Modify one element of the matrix.
  void modifyCoefficient(int row, int column,
  			 double val, bool keepZero = false) ;

  /// Check if status array exists.
  bool statusExists() const ;
  /// Get the status array (numberRows + numberColumns entries)
  unsigned char *statusArray() const ;
  /// Load a status array.
  void copyinStatus(const unsigned char *statusArray) ;
  /// Get basis info for a variable.
  ClpSimplexAPI::Status getColumnStatus(int index) ;
  /// Set status for a variable
  void setColumnStatus(int index, ClpSimplexAPI::Status status) ;
  /// Retrieve the status for a row.
  ClpSimplexAPI::Status getRowStatus(int index) ;
  /// Set status for a row.
  void setRowStatus (int index, ClpSimplexAPI::Status status) ;

  /// Maximum length of names (0 means no names are available).
  int lengthNames() const ;
  /// Get row name (size of buffer must be at least #lengthNames()+1)
  void rowName(int index, char *buffer) const ;
  /// Set row name (null-terminated string).
  void setRowName(int index, const char *buffer) ;
  /// Get column name (size of buffer must be at least #lengthNames()+1)
  void columnName(int index, char *buffer) const ;
  /// Set column name (null-terminated string).
  void setColumnName(int index, const char *buffer) ;
  /// Copy in row & column names.
  void copyNames(const char *const *rowNames, const char *const *colNames) ;
  /// Drop names.
  void dropNames() ;
//@}

/*! \name Methods to read or write a problem file */
//@{
  /// Read an mps file from the given filename.
  int readMps(const char *filename, bool keepNames = false,
	      bool ignoreErrors = false) ;
  /// Write an mps file to the given filename.
  int writeMps(const char *filename, int formatType,
  	       int numberAcross, double objSense) ;
  /// Save the model to a file.
  int saveModel(const char *fileName) ;
  /// Restore the model from a file.
  int restoreModel(const char *fileName) ;
//@}

/*! \name Parameter gets & sets */
//@{
  /// Primal zero tolerance
  double primalTolerance() const ;
  void setPrimalTolerance(double val) ;

  /// Dual zero tolerance
  double dualTolerance() const ;
  void setDualTolerance(double val) ;

  /// Dual objective limit
  double dualObjectiveLimit() const ;
  void setDualObjectiveLimit(double val) ;

  /// Dual bound
  double dualBound() const ;
  void setDualBound(double val) ;

  /// Scaling mode
  int scalingFlag() const ;
  void scaling(int mode) ;

  /*! \brief objective sense

    -1 to maximise, 1 to minimise
  */
  double objSense() const ;
  void setObjSense(double val) ;

  /// Constant offset for objective
  double objectiveOffset() const ;
  void setObjectiveOffset(double val) ;

  /// Get problem name
  std::string problemName() const ;
  /// Set problem name
  void setProblemName(std::string probName) ;

  /// Number of iterations
  int numberIterations() const ;
  void setNumberIterations(int val) ;

  /// Maximum number of iterations
  int maximumIterations() const ;
  void setMaximumIterations(int val) ;

  /// Maximum number of seconds
  double maximumSeconds () const ;
  void setMaximumSeconds (double val) ;

  /// Infeasibility cost
  double infeasibilityCost() const ;
  void setInfeasibilityCost(double val) ;

  /// Problem perturbation
  int perturbation() const ;
  void setPerturbation(int val) ;

  /// Solve algorithm
  int algorithm() const ;
  void setAlgorithm(int val) ;

  /// Small element value
  double getSmallElementValue() const ;
  void setSmallElementValue(double val) ;
//@}

/*! \name Bulk parameter manipulation

  Methods for getting and setting parameters in a way that matches OSI2
  standard parameter management. Also methods to work with a RunParamsAPI
  object to get and set many parameters in a single operation.
*/
//@{
  /// Get / set an integer parameter
  int getIntParam(std::string name) const ;
  void setIntParam(std::string name, int val) ;
  /// Get / set a double parameter
  double getDblParam(std::string name) const ;
  void setDblParam(std::string name, double val) ;
  /// Get / set a string parameter
  std::string getStrParam(std::string name) const ;
  void setStrParam(std::string name, std::string val) ;

  /// Fill a RunParamsAPI object with parameters and current values
  void exposeParams(RunParamsAPI &runParams) const ;
  /// Load parameters from a RunParamsAPI object
  void loadParams(RunParamsAPI &runParams) ;
//@}

/*! \name Information about the solution*/
//@{
  /// Status of problem
  int status() const ;
  void setProblemStatus(int status) ;
  /// Secondary status of problem
  int secondaryStatus() const ;
  void setSecondaryStatus(int status) ;
  /// Is the problem primal feasible?
  bool primalFeasible() const ;
  /// Is the problem dual feasible?
  bool dualFeasible() const ;
  /// Was the problem abandoned due to numerical difficulties?
  bool isAbandoned() const ;
  /// Did the solver hit the maximum iteration or time limits?
  bool hitMaximumIterations() const ;
  /// Was the iteration limit reached?
  bool isIterationLimitReached() const ;
  /// Is optimality proven?
  bool isProvenOptimal() const ;
  /// Is primal infeasibility proven?
  bool isProvenPrimalInfeasible() const ;
  /// Is dual infeasibility proven?
  bool isProvenDualInfeasible() const ;

  /// Objective function value
  double objectiveValue() const ;

  /// Primal row solution
  const double *getRowActivity() const ;
  /// Primal column solution
  const double *getColSolution() const ;
  /// Set the primal column solution
  void setColSolution(const double *soln) ;

  /// Dual row solution
  const double *getRowPrice() const ;
  /// Dual column solution
  const double *getReducedCost() const ;

  /// Unbounded ray (null if bounded)
  double *unboundedRay() const ;
  /// Infeasible ray (null if feasible)
  double *infeasibilityRay() const ;
  /// Free a ray
  void freeRay(double *ray) const ;

  // /// Number of primal infeasibilities
  int numberPrimalInfeasibilities() const ;
  /// Sum of primal infeasibilities
  double sumPrimalInfeasibilities() const ;

  /// Number of dual infeasibilities
  int numberDualInfeasibilities() const ;
  /// Sum of dual infeasibilities
  double sumDualInfeasibilities() const ;

  /// Check the solution
  void checkSolution() ;
//@}

/*! \name User pointer */
//@{
  /// Set the user pointer
  void setUserPointer(void *pointer) ;
  /// Get the user pointer
  void *getUserPointer() const ;
//@}

/*! \name Message handling */
//@{
  /// Pass in a callback function
  void registerCallBack(CallBack userCallBack) ;
  /// Remove a callback function
  void clearCallBack() ;
  /// Set the log level
  void setLogLevel(int value) ;
  /// Get the log level
  int logLevel() const ;
//@}

/*! \name Functions most useful to user */
//@{
  /// Default initial solve
  int initialSolve() ;
  /// Dual initial solve
  int initialDualSolve() ;
  /// Primal initial solve
  int initialPrimalSolve() ;
  /// Barrier initial solve
  int initialBarrierSolve() ;
  /// Barrier initial solve, no follow on crossover
  int initialBarrierNoCrossSolve() ;
  /// Dual algorithm
  int dual (int ifValuesPass = 0) ;
  /// Primal algorithm
  int primal (int ifValuesPass = 0 ) ;
  /// Basis crash
  int crash (double gap, int pivot) ;
//@}

/*! \name API management & enquiry */
//@{
  inline int getAPIs(const char **&idents)
  { return (paramMgr_.getAPIs(idents)) ; }

  inline void *getAPIPtr (const char *ident)
  { return (paramMgr_.getAPIPtr(ident)) ; }

private:
  /*! \name Dynamic object management information */
  //@{
    /// Parameter management object
    ParamBEAPI_Imp<ClpSimplexAPI_ClpLite> paramMgr_ ;
    /// Dynamic library handle
    DynamicLibrary *libClp_ ;
    /// Clp object
    Clp_Simplex *clpC_ ;
  //@}

  /*! \name Auxilliary methods */
  //@{
    /// Get problem name using struct {int,char*}
    void getProbNameVoid(void *blob) const ;
    /// Set problem name using struct {int,char*}
    void setProbNameVoid(const void *blob) ;
  //@}
} ;

}  // end namespace Osi2

#endif

