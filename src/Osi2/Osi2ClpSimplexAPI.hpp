#ifndef Osi2ClpSimplexAPI_HPP
# define Osi2ClpSimplexAPI_HPP

#include <vector>

#include "Coin_C_defines.h"

namespace Osi2 {

class RunParamsAPI ;

/*! \brief Provide the ClpSimplex `C' interface

  This %API exposes the portion of the ClpSimplex class provided by the Clp
  `C' interface.
*/
class ClpSimplexAPI : public API {

public:

  /// API ID
  inline static const char *getAPIIDString() { return ("ClpSimplex") ; }

  /*! \brief variable status

    First 4 match CoinWarmStartBasis,
    isFixed means fixed at lower bound and out of basis
  */
  enum Status {
    isFree = 0x00,
    basic = 0x01,
    atUpperBound = 0x02,
    atLowerBound = 0x03,
    superBasic = 0x04,
    isFixed = 0x05
  } ;

  /// fake bounds for dual
  enum FakeBound {
    noFake = 0x00,
    lowerFake = 0x01,
    upperFake = 0x02,
    bothFake = 0x03
  } ;

  /*! \name Constructors, destructor, and copy

    Because construction is handled through ControlAPI, we don't need a
    constructor. Arguably, we should not need a destructor, either.

    \todo See if it's possible to get rid of the destructor; at least, make it
    protected.
  */
  //@{

  /// Destructor
  virtual ~ClpSimplexAPI() { }
  //@}

  /*! \name Methods to load, modify, and retrieve a problem */
  //@{
  /*! \brief Load a problem

    The matrix is given in a standard column major ordered format (without
    gaps).
    The constraints on the rows are given by lower and upper bounds. If a
    pointer is 0 then the following values are the default:
    <ul>
      <li> <code>colub</code>: all columns have upper bound infinity
      <li> <code>collb</code>: all columns have lower bound 0
      <li> <code>rowub</code>: all rows have upper bound infinity
      <li> <code>rowlb</code>: all rows have lower bound -infinity
      <li> <code>obj</code>: all variables have 0 objective coefficient
    </ul>
  */
  virtual void loadProblem(const int numcols, const int numrows,
    const CoinBigIndex *start, const int *index, const double *value,
    const double *collb, const double *colub,
    const double *obj,
    const double *rowlb, const double *rowub) = 0 ;

  /// read quadratic part of the objective (the matrix part)
  virtual void loadQuadraticObjective(const int numberColumns,
    const CoinBigIndex *start, const int *index, const double *value) = 0 ;

  /// Get integer information
  virtual char *integerInformation() const = 0 ;
  /// Add integer information: 0 for continuous, 1 for integer
  virtual void copyInIntegerInformation(const char *information) = 0 ;
  /// Drop integer information
  virtual void deleteIntegerInformation() = 0 ;

  /// Resize rim part of model
  virtual void resize(int newNumberRows, int newNumberColumns) = 0 ;

  /// Get number of rows
  virtual int numberRows() = 0 ;
  /// Add rows
  virtual void addRows(int number,
    const double *rowLower, const double *rowUpper,
    const CoinBigIndex *rowStart, const int *index, const double *value) = 0 ;
  /// Delete rows
  virtual void deleteRows(int number, const int *which) = 0 ;

  /// Get number of columns
  virtual int numberColumns() = 0 ;
  /// Add columns
  virtual void addColumns(int number,
    const double *columnLower, const double *columnUpper,
    const double *objective,
    const CoinBigIndex *columnStarts, const int *rows,
    const double *elements) = 0 ;
  /// Delete columns
  virtual void deleteColumns(int number, const int *which) = 0 ;

  /// Get row lower bounds
  virtual double *rowLower() const = 0 ;
  /// Change row lower bounds
  virtual void chgRowLower(const double *rowLower) = 0 ;

  /// Get row upper bounds
  virtual double *rowUpper() const = 0 ;
  /// Change row upper bounds
  virtual void chgRowUpper(const double *rowUpper) = 0 ;

  /// Get column lower bounds
  virtual double *columnLower() const = 0 ;
  /// Change column lower bounds
  virtual void chgColumnLower(const double *columnLower) = 0 ;

  /// Get column upper bounds
  virtual double *columnUpper() const = 0 ;
  /// Change column upper bounds
  virtual void chgColumnUpper(const double *columnUpper) = 0 ;
  
  /// Get objective coefficients
  virtual double *objective() const = 0 ;
  /// Change objective coefficients
  virtual void chgObjCoefficients(const double *objIn) = 0 ;

  /// Number of elements in matrix
  virtual CoinBigIndex getNumElements() const = 0 ;
  /// Column starts in matrix
  virtual const CoinBigIndex *getVectorStarts() const = 0 ;
  /// Column lengths in matrix
  virtual const int *getVectorLengths() const = 0 ;
  /// Row indices in matrix
  virtual const int *getIndices() const = 0 ;
  /// element values in matrix
  virtual const double *getElements() const = 0 ;
  /// Modify one element of a matrix
  virtual void modifyCoefficient(int row, int column, double newElement,
    bool keepZero = false) = 0 ;

  /// Check if status array exists
  virtual bool statusExists() const = 0 ;
  /// Get address of status array (char[numberRows+numberColumns])
  virtual unsigned char *statusArray() const = 0 ;
  /// Copy in a status vector
  virtual void copyinStatus(const unsigned char *statusArray) = 0 ;
  /// Get basis info for a variable.
  virtual ClpSimplexAPI::Status getColumnStatus(int sequence) = 0 ;
  /*! \brief Set status for a variable 

    Also force the value if the status is at bound.
  */
  virtual void setColumnStatus(int sequence,
  			       ClpSimplexAPI::Status newStatus) = 0 ;
  /// Get status for a row
  virtual ClpSimplexAPI::Status getRowStatus(int sequence) = 0 ;
  /*! \brief Set status for a row 

    Also force the value if the status is at bound.
  */
  virtual void setRowStatus(int sequence,
  			    ClpSimplexAPI::Status newStatus) = 0 ;

  /// length of names (0 means no names available)
  virtual int lengthNames() const = 0 ;
  /// Get row name (size of name must be at least #lengthNames()+1)
  virtual void rowName(int iRow, char *name) const = 0 ;
  /// Set row name (name should be null-terminated)
  virtual void setRowName(int iRow, const char *name) = 0 ;
  /// Get column name (size of name must be at least #lengthNames()+1)
  virtual void columnName(int iCol, char *name) const = 0 ;
  /// Set column name (name should be null-terminated)
  virtual void setColumnName(int iCol, const char *name) = 0 ;
  /// Copy in row & column names
  virtual void copyNames(const char *const *rowNames,
  			 const char *const *columnNames) = 0 ;
  /// Drop names
  virtual void dropNames() = 0 ;

  //@}

  /*! \name Methods to read or write a problem file */
  //@{
  /// Read an MPS file from the given filename
  virtual int readMps(const char *filename,
    bool keepNames = false,
    bool ignoreErrors = false) = 0 ; 

  /*! \brief Write an MPS file to the given filename

    Format type is 0 = normal, 1 = extra or 2 = hex.
    Number across is 1 or 2.
    Use objSense = -1D to flip the objective function around.
  */
  virtual int writeMps(const char *filename, int formatType,
  		       int numberAcross, double objSense) = 0 ;

  /// Save the model to a file
  virtual int saveModel(const char *fileName) = 0 ;
  /// Restore model from a file
  virtual int restoreModel(const char *fileName) = 0 ;
  //@}

  /*! \name Parameter Gets & Sets */
  //@{
  /// Primal zero tolerance
  virtual double primalTolerance() const = 0 ;
  virtual void setPrimalTolerance(double value) = 0 ;

  /// Dual zero tolerance
  virtual double dualTolerance() const = 0 ;
  virtual void setDualTolerance(double value) = 0 ;

  /// Dual objective limit
  virtual double dualObjectiveLimit() const = 0 ;
  virtual void setDualObjectiveLimit(double value) = 0 ;

  /// Dual bound
  double dualBound() ;
  void setDualBound(double value) ;

  /*! \brief Select scaling mode

    0 - off
    1 - equilibrium
    2 - geometric
    3 - auto
    4 - dynamic (implemented?)
  */
  virtual void scaling(int mode) = 0 ;
  /// Get scaling mode
  virtual int scalingFlag() const = 0 ;

  /*! \brief Direction of optimisation

    -1.0  maximise
     0.0  ignore
     1.0  minimise
  */
  virtual double objSense() const = 0 ;
  virtual void setObjSense(double value) = 0 ;

  /// Constant offset for objective
  virtual double objectiveOffset() const = 0 ;
  virtual void setObjectiveOffset(double value) = 0 ;

  /// Get problem name
  virtual std::string problemName() const = 0 ;
  /// Set problem name
  virtual void setProblemName(std::string probName) = 0 ;

  /// Number of iterations
  virtual int numberIterations() const = 0 ;
  virtual void setNumberIterations(int value) = 0 ;
  /// Maximum number of iterations
  virtual int maximumIterations() const = 0 ;
  virtual void setMaximumIterations(int value) = 0 ;
  /// Maximum time in seconds (timed from call to setMaximumSeconds)
  virtual double maximumSeconds() const = 0 ;
  virtual void setMaximumSeconds(double value) = 0 ;

  /// Infeasibility cost
  virtual double infeasibilityCost() const = 0 ;
  virtual void setInfeasibilityCost(double value) = 0 ;

  /*! \brief Problem perturbation

     50 - switch on perturbation
    100 - auto perturb if takes too long (1.0e-6 largest nonzero)
    101 - problem is perturbed
    102 - do not try perturbing again

    Default is 100.
  */
  virtual int perturbation() const = 0 ;
  virtual void setPerturbation(int value) = 0 ;

  /// Get solve algorithm
  virtual int algorithm() const = 0 ;
  /// Set solve algorithm
  virtual void setAlgorithm(int value) = 0 ;

  /*! \brief Get small element value
    Elements less that this are set to zero. Default 1.0e-20.
  */
  virtual double getSmallElementValue() const = 0 ;
  /// Set small element value
  virtual void setSmallElementValue(double value) = 0 ;
  //@}

  /*! \name Bulk parameter manipulation

    These methods work with a RunParamsAPI object to maintain a complete set
    of parameters that can be applied in one operation.
  */
  //@{
  /// Load a runParams object with parameters.
  virtual void exposeParams(RunParamsAPI &runParams) const = 0 ;

  /// Set ClpSimplex parameters from a RunParamsAPI object.
  virtual void loadParams(RunParamsAPI &runParams) = 0 ;
  //@}

  /*! \name Information about the solution */
  //@{
  /*! \brief Status of problem:

       0 - optimal
       1 - primal infeasible
       2 - dual infeasible
       3 - stopped on iterations etc
       4 - stopped due to errors
  */
  virtual int status() const = 0 ;
  /** Set problem status */
  virtual void setProblemStatus(int problemStatus) = 0 ;

  /*! \brief Secondary status of problem - may get extended

    0 - none
    1 - primal infeasible because dual limit reached
    2 - scaled problem optimal - unscaled has primal infeasibilities
    3 - scaled problem optimal - unscaled has dual infeasibilities
    4 - scaled problem optimal - unscaled has dual and primal infeasibilities
  */
  virtual int secondaryStatus() const = 0 ;
  virtual void setSecondaryStatus(int status) = 0 ;

  /// Problem is primal feasible
  virtual bool primalFeasible() const = 0 ;
  /// Problem is dual feasible
  virtual bool dualFeasible() const = 0 ;
  /// Are there numerical difficulties?
  virtual bool isAbandoned() const = 0 ;
  /// Return true if hit maximum iterations or time
  virtual bool hitMaximumIterations() const = 0 ;
  /// Iteration limit reached?
  virtual bool isIterationLimitReached() const = 0 ;
  /// Is optimality proven?
  virtual bool isProvenOptimal() const = 0 ;
  /// Is primal infeasibility proven?
  virtual bool isProvenPrimalInfeasible() const = 0 ;
  /// Is dual infeasibility proven?
  virtual bool isProvenDualInfeasible() const = 0 ;
  /// Is the given primal objective limit reached

  /// Objective function value
  virtual double objectiveValue() const = 0 ;

  /// Primal row solution
  virtual const double *getRowActivity() const = 0 ;
  /// Primal column solution
  virtual const double *getColSolution() const = 0 ;
  /// Set the primal column solution
  virtual void setColSolution(const double *input) = 0 ;

  /// Dual row solution
  virtual const double *getRowPrice() const = 0 ;
  /// Dual column solution
  virtual const double *getReducedCost() const = 0 ;

  /*! \brief Unbounded ray (null if bounded)

    Use #freeRay to free the returned ray.
  */
  virtual double *unboundedRay() const = 0 ;
  /*! Infeasible ray (null if feasible)

    Use #freeRay to free the returned ray.
  */
  virtual double *infeasibilityRay() const = 0 ;
  /*! \brief Free a ray

    Use this method to free rays returned by #unboundedRay and
    #infeasibilityRay.
  */
  virtual void freeRay(double *ray) const = 0 ;

  /// Number of primal infeasibilities
  virtual int numberPrimalInfeasibilities() const = 0 ;
  /// Sum of primal infeasibilities
  virtual double sumPrimalInfeasibilities() const = 0 ;

  /// Number of dual infeasibilities
  virtual int numberDualInfeasibilities() const = 0 ;
  /// Sum of dual infeasibilities
  virtual double sumDualInfeasibilities() const = 0 ;

  /*! Check the solution

    Sets sum of infeasibilities, etc.
  */
  virtual void checkSolution() = 0 ;
  //@}
  
  /*! \name User pointer
  
    Reserved for use by the application, for whatever purpose.
  */
  //@{
  /// Set the user pointer
  virtual void setUserPointer(void *pointer) = 0 ;
  /// Get the user pointer
  virtual void *getUserPointer() const = 0 ;
  //@}

  /*! \name Message handling

    Callbacks are handled by ONE function.
  */
  //@{
  /// Typedef for callbacks
  typedef clp_callback CallBack ;
  /// Pass in callback function
  virtual void registerCallBack(CallBack userCallBack) = 0 ;
  /// Unset callback function
  virtual void clearCallBack() = 0 ;
  /// Set the log level
  virtual void setLogLevel(int value) = 0 ;
  /// Get the log level
  virtual int logLevel() const = 0 ;
  //@}


  /*! \name Functions most useful to user */
  //@{
  /// Default initial solve
  virtual int initialSolve() = 0 ;
  /*! \brief General solve algorithm which can do presolve.

     See  ClpSolve.hpp for options
  */
  // int initialSolve(ClpSolve &options) ;
  /// Dual initial solve
  virtual int initialDualSolve() = 0 ;
  /// Primal initial solve
  virtual int initialPrimalSolve() = 0 ;
  /// Barrier initial solve
  virtual int initialBarrierSolve() = 0 ;
  /// Barrier initial solve, not to be followed by crossover
  virtual int initialBarrierNoCrossSolve() = 0 ;
  /*! \brief Dual algorithm

    See ClpSimplexDual.hpp for method.
    ifValuesPass==2 just does values pass and then stops.
  */
  virtual int dual(int ifValuesPass = 0) = 0 ;
  /*! \brief Primal algorithm

   See ClpSimplexPrimal.hpp for method.
   ifValuesPass==2 just does values pass and then stops.
  */
  virtual int primal(int ifValuesPass = 0) = 0 ;

  /*! \brief Crash a basis

    At present, just aimed at dual simplex. Return codes are
    -2 - dual preferred, crash basis created
    -1 - dual preferred, slack basis preferred
     0 - basis going in was not all slack
     1 - primal preferred, slack basis preferred
     2 - primal preferred, crash basis created

   If the gap between bounds <= \p gap, variables can be flipped.

   The values for pivot are
     0 - no pivoting allowed, choice of algorithm only
     1 - simple pivoting
     2 - mini iterations
  */
  virtual int crash(double gap,int pivot) = 0 ;
  //@}

} ;

}    // end namespace Osi2

#endif

