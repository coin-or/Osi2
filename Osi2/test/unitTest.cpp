/**
 *
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin, 
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 */ 
 


#include <OsiSolverInterface.hpp>   
#include <CoinMessageHandler.hpp> 
#include <CoinPackedMatrix.hpp>
#include "OsiClpSolverInterface.hpp" 
  

#include "OSResult.h"
#include "OSiLReader.h"       
#include "OSiLWriter.h" 
#include "OSrLReader.h"        
#include "OSrLWriter.h"   
#include "OSInstance.h"  
#include "FileUtil.h"  

//#include "OSConfig.h" 

#include "CoinSolver.h"
#include "DefaultSolver.h"  
#include "WSUtil.h" 
#include "OSSolverAgent.h"   
#include "OShL.h"     
#include "ErrorClass.h"
#include "OSmps2osil.h"   
#include "Base64.h"
#ifdef COIN_HAS_ASL
#include "OSnl2osil.h"
#endif
#ifdef COIN_HAS_LINDO    
#include "LindoSolver.h"
#endif  

#include "OsiXFormMgr.h"
#include "OsiXForm.h"
 
#include <time.h>
#include<sstream>
#include<iostream>
  
 

int main(int argC, char* argV[])
{

	std::cout << "START UNIT TEST" << std::endl; 

	FileUtil *fileUtil = NULL;  
	std::string osilFileName;
	std::string osil;
	std::string osol = "<osol></osoL>";
	const char dirsep =  CoinFindDirSeparator();
  	std::string dataDir;
    dataDir = dirsep == '/' ? "../data/" : "..\\data\\";
	osilFileName =  dataDir + "parincLinear.osil";
	// get the data from the test file
	osil = fileUtil->getFileAsString( &osilFileName[0]);
	//
	// create an instance object
	OSiLReader *osilreader = NULL;
	OSInstance *osinstance = NULL;
	osilreader = new OSiLReader(); 
	osinstance = osilreader->readOSiL( &osil);
	// now create a solver object
	DefaultSolver *m_Solver  = NULL;
	m_Solver = new CoinSolver();
	// let's use cbc
	m_Solver->m_sSolverName = "cbc";
	// set the solver options
	m_Solver->osol = osol;  
	// give the solver the instance object
	m_Solver->osinstance = osinstance;
	//now solve
	m_Solver->solve();
	cout << "Here is the COIN Cbc solver solution for parincLinear" << endl;
	cout << m_Solver->osrl << endl;
	// set up the transformer modification manager
 	OsiXFormMgr* mod;
 	mod = new OsiXFormMgr();
 	// modify the constraints in the problem
 	int number = 4;
 	std::string* names = NULL;
 	double* lowerBounds = NULL; 
 	double* upperBounds = NULL; 
 	upperBounds = new double[4];
 	upperBounds[ 0] = 500;
 	upperBounds[1] = 200;
 	upperBounds[2] = 100;
 	upperBounds[3] = 129;
  	lowerBounds = new double[4];
 	lowerBounds[ 0] = 0;
 	lowerBounds[1] = 0;
 	lowerBounds[2] = 0;
 	lowerBounds[3] = 0;
 	double* constants = NULL;
	// ModifyConstraintBounds is a class that inherits from the virtual calls OsiXForm
	// record the constraint modification as an object
	// we are using the GoF Command Design Pattern
 	mod->record(new ModifyConstraintBounds(osinstance, number, names, lowerBounds, upperBounds, constants) );
 	// now make the modification
 	mod->callXFormClasses();
 	//
 	// now solve again
 	m_Solver->osinstance = osinstance;
 	m_Solver->solve();
	cout << "Here is the COIN Cbc solver solution for parincLinear" << endl;
	cout << m_Solver->osrl << endl;
 	cout << endl << endl << endl;
 	delete m_Solver;
	m_Solver = NULL;
	return 0;
}
