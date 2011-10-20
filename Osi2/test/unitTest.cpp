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

#include "Osi2ControlAPI_Imp.hpp"
#include "Osi2ProbMgmtAPI.hpp"

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
  for a WildProgMgmt object.

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
      Invoke createObject. If it works, try to invoke a nontrivial method. Which
      will fail, because exmip1 is not available, but that's not the point. Then
      destroy the object.
    */
    DummyAdapter dummy ;
    ProbMgmtAPI *clp =
        static_cast<ProbMgmtAPI *>(plugMgr.createObject("ProbMgmt", 0, dummy)) ;
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
    ProbMgmtAPI *bogus =
        static_cast<ProbMgmtAPI *>(plugMgr.createObject("BogusAPI", 0, dummy)) ;
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
    clp = static_cast<ProbMgmtAPI *>(plugMgr.createObject("WildProbMgmt",
                                     0, dummy)) ;
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

int testControlAPI (const std::string &shortName,
		    const std::string &dfltSampleDir)
{
    int retval = 0 ;
    int errcnt = 0 ;
    std::cout
            << "Attempting to instantiate a ControlAPI_Imp object." << std::endl ;
    ControlAPI_Imp ctrlAPI ;
    std::cout << "Log level is " << ctrlAPI.getLogLvl() << std::endl ;
    /*
      Load shims. No sense proceeding if the load fails.
    */
    retval = ctrlAPI.load(shortName) ;
    if (retval != 0) {
        errcnt++ ;
        std::cout
                << "Apparent failure to load " << shortName << "." << std::endl ;
        std::cout
                << "Error code is " << retval << "." << std::endl ;
        return (errcnt) ;
    }
    std::string heavyName = shortName+"Heavy" ;
    retval = ctrlAPI.load(heavyName) ;
    if (retval != 0) {
        errcnt++ ;
        std::cout
	    << "Apparent failure to load " << heavyName << "." << std::endl ;
        std::cout
	    << "Error code is " << retval << "." << std::endl ;
        return (errcnt) ;
    }
    /*
      Create a ProbMgmt object, invoke a nontrivial method, and destroy the
      object.
    */
    API *apiObj = nullptr ;
    retval = ctrlAPI.createObject(apiObj, "ProbMgmt") ;
    if (retval != 0) {
        errcnt++ ;
        std::cout
                << "Apparent failure to create a ProbMgmt object." << std::endl ;
    } else {
        ProbMgmtAPI *clp = dynamic_cast<ProbMgmtAPI *>(apiObj) ;
	std::string exmip1Path = dfltSampleDir+"/brandy.mps" ;
        clp->readMps(exmip1Path.c_str(), true) ;
	clp->initialSolve() ;
        int retval = ctrlAPI.destroyObject(apiObj, "ProbMgmt", 0) ;
        if (retval < 0) {
            errcnt++ ;
            std::cout
                    << "Apparent failure to destroy a ProbMgmt object." << std::endl ;
        }
    }
    /*
      Create a ProbMgmt object from ClpHeavy, invoke a nontrivial method,
      and destroy the object.
    */
    apiObj = nullptr ;
    retval = ctrlAPI.createObject(apiObj, "ProbMgmt", &heavyName) ;
    if (retval != 0) {
        errcnt++ ;
        std::cout
	    << "Apparent failure to create a ProbMgmt (heavy) object." << std::endl ;
    } else {
        ProbMgmtAPI *clp = dynamic_cast<ProbMgmtAPI *>(apiObj) ;
	std::string exmip1Path = dfltSampleDir+"/brandy.mps" ;
        clp->readMps(exmip1Path.c_str(), true) ;
	clp->initialSolve() ;
        int retval = ctrlAPI.destroyObject(apiObj, "ProbMgmt", 0) ;
        if (retval < 0) {
            errcnt++ ;
            std::cout
                    << "Apparent failure to destroy a ProbMgmt (heavy) object." << std::endl ;
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
        ProbMgmtAPI *clp = dynamic_cast<ProbMgmtAPI *>(apiObj) ;
        clp->readMps("exmip1.mps", true) ;
        int retval = ctrlAPI.destroyObject(apiObj, "ProbMgmt", &shortName) ;
        if (retval < 0) {
            errcnt++ ;
            std::cout
                    << "Apparent failure to destroy a restricted ProbMgmt object."
                    << std::endl ;
        }
    }
    /*
      Unload the shims.
    */
    retval = ctrlAPI.unload(shortName) ;
    if (retval != 0) {
        errcnt++ ;
        std::cout
                << "Apparent failure to unload " << shortName << "." << std::endl ;
        std::cout
                << "Error code is " << retval << "." << std::endl ;
    }
    retval = ctrlAPI.unload(heavyName) ;
    if (retval != 0) {
        errcnt++ ;
        std::cout
                << "Apparent failure to unload " << heavyName << "." << std::endl ;
        std::cout
                << "Error code is " << retval << "." << std::endl ;
    }

    return (errcnt) ;
}

} // end unnamed file-local namespace


int main(int argC, char* argV[])
{

    std::cout << "START UNIT TEST" << std::endl ;

    std::string dfltSampleDir = "../../Data/Sample" ;
    /*
      Test the bare PluginManager. There's no sense proceeding to the API tests if
      the PluginManager isn't working.
    */
    std::cout << "Testing bare PluginManager." << std::endl ;
    int retval = testPluginManager("libOsi2ClpShim.so") ;
    std::cout
            << "End test of bare PluginManager, " << retval << " errors."
            << std::endl << std::endl ;
    if (retval != 0) {
        std::cout << "Aborting unitTest; errors in PluginManager." << std::endl ;
        return (retval) ;
    }
    /*
      Now let's try the Osi2 control API.
    */
    std::cout << "Testing ControlAPI." << std::endl ;
    retval = testControlAPI("clp",dfltSampleDir) ;
    std::cout
            << "End test of ControlAPI, " << retval << " errors."
            << std::endl << std::endl ;
    /*
      Shut down the plugin manager. This will call the plugin library exit
      functions and unload the libraries.
    */
    PluginManager &plugMgr = PluginManager::getInstance() ;
    std::cout
            << "Shutting down plugins (executing exit functions)." << std::endl ;
    plugMgr.shutdown() ;

    std::cout << "END UNIT TEST" << std::endl ;

    return (retval) ;

}

