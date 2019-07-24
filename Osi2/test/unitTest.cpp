/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$

  This file contains the unit test for the OSI2 PluginManager and OSI2 APIs.
*/

#include "CoinHelperFunctions.hpp"


#include "Osi2Config.h"
#include "Osi2nullptr.hpp"
#include "Osi2PluginManager.hpp"
#include "Osi2DynamicLibrary.hpp"
#include "Osi2ObjectAdapter.hpp"

#include "Osi2ParamMgmtAPI_Imp.hpp"
#include "Osi2ControlAPI_Imp.hpp"
#include "Osi2ProbMgmtAPI.hpp"
#include "Osi2Osi1API.hpp"

using namespace Osi2 ;

namespace {

/*
  Test the bare PluginManager API:
    * Initialise the PluginManager.
    * Load a plugin library.
    * Create ProbMgmt objects: exact match and wild card. Also check that
      we fail correctly for a nonexistent object.

  The test is (sort of) clp-specific, but only in the sense that the test clp
  plugin will return a ProbMgmt object via the wildcard mechanism when asked
  for a WildProbMgmt object.

  Over time, this test should be expanded and broken out into a separate file.
  Arguably, it should be a separate executable.
*/
int testPluginManager (const std::string libName)
{
    int errcnt = 0 ;

    std::cout << "Instantiating PluginManager." << std::endl ;

    PluginManager &plugMgr = PluginManager::getInstance() ;
    /*
      Now let's try to load the shim.
    */
    std::string dfltDir = plugMgr.getDfltPluginDir() ;
    char dirSep = CoinFindDirSeparator() ;
    std::string shimPath = dfltDir + dirSep + libName ;

    int retval = plugMgr.loadOneLib(libName) ;

    if (retval != 0) {
        errcnt++ ;
        std::cout
	  << "Apparent failure to load " << shimPath << "." << std::endl ;
        std::cout
	  << "Error code is " << retval << "." << std::endl ;
        return (errcnt) ;
    }
    /*
      Invoke createObject. If it works, try to invoke a nontrivial
      method. Which will fail, because exmip1 is not available, but that's
      not the point. Then destroy the object.
    */
    DummyAdapter dummy ;
    PluginUniqueID libID = 0 ;
    ProbMgmtAPI *clp =
      static_cast<ProbMgmtAPI *>(plugMgr.createObject("ProbMgmt",libID,dummy)) ;
    if (clp == nullptr) {
        errcnt++ ;
        std::cout
	  << "Apparent failure to create a ProbMgmt object." << std::endl ;
    } else {
        clp->readMps("exmip1.mps", true) ;
        int retval = plugMgr.destroyObject("ProbMgmt", 0, clp) ;
        if (retval < 0) {
            errcnt++ ;
            std::cout
	      << "Apparent failure to destroy a ProbMgmt object." << std::endl ;
        }
        clp = nullptr ;
    }
    /*
      Ask for a nonexistent API and check that we (correctly) fail to provide
      one.
    */
    libID = 0 ;
    ProbMgmtAPI *bogus =
      static_cast<ProbMgmtAPI *>(plugMgr.createObject("BogusAPI",libID,dummy)) ;
    if (bogus == nullptr) {
        std::cout
	  << "Apparent failure to create a BogusAPI object (expected)."
	  << std::endl ;
    } else {
        errcnt++ ;
        std::cout
	  << "Eh? We shouldn't be able to create a BogusAPI object!"
	  << std::endl ;
    }
    /*
      Check that we can create an object through the wildcard mechanism.
    */
    libID = 0 ;
    clp = static_cast<ProbMgmtAPI *>(plugMgr.createObject("WildProbMgmt",
                                     libID, dummy)) ;
    if (clp == nullptr) {
        errcnt++ ;
        std::cout
                << "Apparent failure to create a WildProbMgmt object." << std::endl ;
    } else {
        clp->readMps("exmip1.mps", true) ;
        int retval = plugMgr.destroyObject("WildProbMgmt", 0, clp) ;
        if (retval < 0) {
            std::cout
                    << "Apparent failure to destroy a WildProbMgmt object." << std::endl ;
        }
        clp = nullptr ;
    }
    /*
      Unload the plugin library.
    */
    retval = plugMgr.unloadOneLib(libName) ;
    if (retval != 0) {
        errcnt++ ;
        std::cout
                << "Apparent failure to unload " << shimPath << "." << std::endl ;
        std::cout
                << "Error code is " << retval << "." << std::endl ;
    }
    /*
      Shut down the plugin manager. This will call the plugin library exit
      functions and unload the libraries.
    */
    std::cout
            << "Shutting down PluginManager (executing exit functions)." << std::endl ;
    retval = plugMgr.shutdown() ;
    if (retval < 0) {
        errcnt++ ;
        std::cout
                << "Apparent failure of PluginManager shutdown." << std::endl ;
    }

    return (errcnt) ;
}

/*
  Test the control API and various aspects of object creation and destruction.
  The method creates various objects, works with them, and finally destroys
  all of them. Part of the test is that there are multiple objects in
  existence, and multiple objects of the same API.
*/
int testControlAPI (const std::string &shortName,
		    const std::string &dfltSampleDir)
{
  int retval = 0 ;
  int errcnt = 0 ;
  std::vector<API *> apiObjs ;
  char dirSep = CoinFindDirSeparator() ;

  std::cout
      << "Attempting to instantiate a ControlAPI_Imp object." << std::endl ;
  ControlAPI_Imp ctrlAPI ;
  std::cout << "Log level is " << ctrlAPI.getLogLvl() << std::endl ;
/*
  Load the shim.
*/
  retval = ctrlAPI.load(shortName) ;
  if (retval != 0) {
    std::cout
	<< "Apparent failure to load " << shortName << "." << std::endl ;
    std::cout
	<< "Error code is " << retval << "." << std::endl ;
  }
/*
  Create a ProbMgmt API object and invoke a nontrivial method.
*/
  API *apiObj = nullptr ;
  retval = ctrlAPI.createObject(apiObj, "ProbMgmt") ;
  if (retval != 0) {
    errcnt++ ;
    std::cout
	<< "Apparent failure to create a ProbMgmt object." << std::endl ;
  } else {
    apiObjs.push_back(apiObj) ;
    ProbMgmtAPI *clp = dynamic_cast<ProbMgmtAPI *>(apiObj) ;
    std::string exmip1Path = dfltSampleDir+dirSep+"brandy.mps" ;
    clp->readMps(exmip1Path.c_str(), true) ;
    clp->initialSolve() ;
  }
/*
  Create an Osi1 object, invoke a nontrivial method, and destroy the
  object. The clone test here is checking several things: First, that the
  cloned object is viable (i.e., the original clone method still works)
  and, second, that the control information block has been stripped. The
  second destroy should fail because there's no control information block.
  (The ControlAPI didn't create this object and takes no responsibility
  for it!)  Later, the destruction of the original object should succeed,
  because it still possesses the control information block.
*/
  apiObj = nullptr ;
  retval = ctrlAPI.createObject(apiObj, "Osi1") ;
  if (retval != 0) {
    errcnt++ ;
    std::cout
	<< "Apparent failure to create an Osi1 object." << std::endl ;
  } else {
    apiObjs.push_back(apiObj) ;
    Osi1API *osi = dynamic_cast<Osi1API *>(apiObj) ;
    std::string exmip1Path = dfltSampleDir+dirSep+"brandy.mps" ;
    osi->readMps(exmip1Path.c_str()) ;
    std::cout << "    cloning ... " << std::endl ;
    Osi1API *o2 = osi->clone() ;
    o2->initialSolve() ;
    if (o2->isProvenOptimal())
      std::cout << "    solved to optimality." << std::endl ;
    apiObj = o2 ;
    retval = ctrlAPI.destroyObject(apiObj) ;
    if (retval < 0) {
	std::cout
	    << "Apparent failure to destroy a cloned Osi1 object"
	    << " (expected)." << std::endl ;
	delete o2 ;
    } else {
      errcnt++ ;
      std::cout
	<< "Invoking ControlAPI::destroyObject on a cloned object"
	<< " should fail!" << std::endl ;
    }
  }
  /*
    Create a restricted ProbMgmt object, invoke a nontrivial method,
    and destroy the object.
  */
  apiObj = nullptr ;
  retval = ctrlAPI.createObject(apiObj, "ProbMgmt", &shortName) ;
  if (retval != 0) {
    errcnt++ ;
    std::cout
	<< "Apparent failure to create a restricted ProbMgmt object."
	<< std::endl ;
  } else {
    apiObjs.push_back(apiObj) ;
    ProbMgmtAPI *clp = dynamic_cast<ProbMgmtAPI *>(apiObj) ;
    clp->readMps("exmip1.mps", true) ;
  }
/*
  Now destroy all the objects.
*/
  for (std::vector<API *>::iterator iter = apiObjs.begin() ;
       iter != apiObjs.end() ;
       iter++) {
    API *apiObj = *iter ;
    int retval = ctrlAPI.destroyObject(apiObj) ;
    if (retval < 0) {
      errcnt++ ;
      std::cout
	<< "Apparent failure to destroy a ProbMgmt object." << std::endl ;
    }
  }
  apiObjs.clear() ;
/*
  Unload the shim.
*/
  retval = ctrlAPI.unload(shortName) ;
  if (retval != 0) {
    errcnt++ ;
    std::cout
	<< "Apparent failure to unload " << shortName << "." << std::endl ;
    std::cout
	<< "Error code is " << retval << "." << std::endl ;
  }
  return (errcnt) ;
}

int testParamMgmtAPI ()
{
  int errCnt = 0 ;

  ParamMgmtAPI_Imp mgmtAPI ;
  std::cout
      << "Instantiated a ParamMgmtAPI_Imp object "
      << std::hex << &mgmtAPI << std::dec << "." << std::endl ;

  ControlAPI_Imp ctrlAPI1 ;
  std::cout
      << "Instantiated a ControlAPI_Imp object "
      << std::hex << &ctrlAPI1 << std::dec << "." << std::endl ;
/*
  Check that the ControlAPI_Imp object thinks that it can implement
  ControlAPI.
*/
  std::vector<const char *>apiStrings ;
  apiStrings.push_back(Osi2::ControlAPI::getAPIIDString()) ;
  apiStrings.push_back(Osi2::ParamBEAPI::getAPIIDString()) ;
  apiStrings.push_back("BogusAPI") ;
  for (int ndx = 0 ; ndx < apiStrings.size() ; ndx++) {
    const char *apiStr = apiStrings[ndx] ;
    void *obj = ctrlAPI1.getAPIPtr(apiStr) ;
    if (obj == nullptr) {
      std::cout
	  << "Control object " << std::hex << &ctrlAPI1 << std::dec
	  << " believes it does not implement " << apiStr << "."
	  << std::endl ;
    } else {
      std::cout
	  << "Control object " << std::hex << &ctrlAPI1 << std::dec
	  << " believes it implements " << apiStr << " with "
	  << std::hex << obj << std::dec << "."
	  << std::endl ;
    }
  }

  std::string ctrlAPI1ID = "CtrlAPI1" ;
  std::cout
      << "Enrolling CtrlAPI1 as \"" << ctrlAPI1ID << "\"" << std::endl ;
  if (!mgmtAPI.enroll(ctrlAPI1ID,&ctrlAPI1)) {
    std::cout
        << "Failed to enroll CtrlAPI1 (" << ctrlAPI1ID << ")" << std::endl ;
    errCnt++ ;
  }
/*
  Attempt to retrieve a parameter value.
*/
  {
    int blob ;
    const char *paramID = "log level" ;
    if (mgmtAPI.get(ctrlAPI1ID,paramID,&blob))
    { std::cout
	<< "The value of " << ctrlAPI1ID << ":" << paramID
	<< " is " << blob << std::endl ;
    } else {
      std::cout
	<< "Failed to get the value of " << ctrlAPI1ID << ":" << paramID
	<< std::endl ;
    }
  /*
    Attempt to set the parameter and retrieve it again.
  */
    blob = blob-2 ;
    if (!mgmtAPI.set(ctrlAPI1ID,paramID,&blob)) {
      std::cout
	<< "Failed to set the value of " << ctrlAPI1ID << ":" << paramID
	<< std::endl ;
    }
    if (mgmtAPI.get(ctrlAPI1ID,paramID,&blob))
    { std::cout
	<< "The value of " << ctrlAPI1ID << ":" << paramID
	<< " is " << blob << std::endl ;
    } else {
      std::cout
	<< "Failed to get the value of " << ctrlAPI1ID << ":" << paramID
	<< std::endl ;
    }
  }
/*
  Attempt to retrieve a parameter value.
*/
  {
    std::string blob ;
    const char *paramID = "DfltPlugDir" ;
    ctrlAPI1.setDfltPluginDir("/home/Coin.Mix/Split-FedGCC/lib") ;
    if (mgmtAPI.get(ctrlAPI1ID,paramID,&blob))
    { std::cout
	<< "The value of " << ctrlAPI1ID << ":" << paramID
	<< " is " << blob << std::endl ;
    } else {
      std::cout
	<< "Failed to get the value of " << ctrlAPI1ID << ":" << paramID
	<< std::endl ;
    }
  }

  return (errCnt) ;
}

} // end unnamed file-local namespace


int main(int argC, char* argV[])
{

    std::cout << "START UNIT TEST" << std::endl ;

    std::string dfltSampleDir = "../../Data/Sample" ;
    /*
      Test the bare PluginManager. There's no sense proceeding to the API
      tests if the PluginManager isn't working.
    */
    std::cout << "Testing bare PluginManager." << std::endl ;
    int retval = testPluginManager("libOsi2ClpShim.so") ;
    std::cout
      << "End test of bare PluginManager, " << retval << " errors."
      << std::endl << std::endl ;
    if (retval != 0) {
        std::cout
	  << "Aborting unitTest; errors in PluginManager." << std::endl ;
        return (retval) ;
    }
    /*
      Now let's try the Osi2 control API.
    */
    typedef std::pair<std::string,int> TestVec ;
    std::vector<TestVec> solvers ;
    solvers.push_back(TestVec("clp",1)) ;
    solvers.push_back(TestVec("clpHeavy",0)) ;
#   ifdef COIN_HAS_OSIGLPK
    solvers.push_back(TestVec("glpkHeavy",2)) ;
#   endif
    std::vector<TestVec>::const_iterator iter ;
    int totalErrs = 0 ;
    for (iter = solvers.begin() ; iter != solvers.end() ; iter++) {
      std::string solverName = iter->first ;
      std::cout
        << "Testing ControlAPI (" << solverName << ")." << std::endl ;
      retval = testControlAPI(solverName,dfltSampleDir) ;
      std::cout
          << "End test of ControlAPI (" << solverName << "), "
	  << retval << " errors, expected " << iter->second << "."
	  << std::endl << std::endl ;
      totalErrs += retval-(iter->second) ;
    }
    /*
      Test the parameter management API.
    */
    int errCnt = 0 ;
    const int expectedErrs = 0 ;
    std::cout << "Testing ParamMgmtAPI." << std::endl ;
    errCnt = testParamMgmtAPI() ;
    std::cout
        << "End test of ParamMgmtAPI, " << errCnt << " errors, expected "
	<< expectedErrs << "." << std::endl ;
    /*
      Shut down the plugin manager. This will call the plugin library exit
      functions and unload the libraries.
    */
    PluginManager &plugMgr = PluginManager::getInstance() ;
    std::cout
      << "Shutting down plugins (executing exit functions)." << std::endl ;
    plugMgr.shutdown() ;

    std::cout << "END UNIT TEST" << std::endl ;

    return (totalErrs) ;

}

