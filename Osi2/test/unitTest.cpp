
// #include "CoinMessageHandler.hpp" 
// #include "CoinPackedMatrix.hpp"


#include "Osi2PluginManager.hpp"
#include "Osi2DynamicLibrary.hpp"
#include "Osi2ObjectAdapter.hpp"

#include "Osi2ProbMgmtAPI.hpp"

using namespace Osi2 ;

// Hedge agaaist C++0x
const int nullptr = 0 ;
  
int main(int argC, char* argV[])
{

  std::cout << "START UNIT TEST" << std::endl ;

  std::cout << "Instantiating PluginManager." << std::endl ;

  PluginManager &plugMgr = PluginManager::getInstance() ;

  /*
    Now let's try to load the shim.
  */
  std::string dfltDir = plugMgr.getDfltPluginDir() ;
  std::string clpShimPath = dfltDir+"/"+"libOsi2ClpShim.so.0" ;
  std::string errMsg ;
  DynamicLibrary *clpShim = DynamicLibrary::load(clpShimPath,errMsg) ;
  if (clpShim == 0) {
    std::cout
      << "Apparent failure opening " << clpShimPath << "." << std::endl ;
    std::cout
      << "Error is " << errMsg << "." << std::endl ;
    return (-1) ;
  }
  /*
    Now let's see if we can invoke the init method
  */
  InitFunc initFunc =
    reinterpret_cast<InitFunc>(clpShim->getSymbol("initPlugin",errMsg)) ;
  if (initFunc == 0) {
    std::cout
      << "Apparent failure to obtain the init method." << std::endl ;
    std::cerr << errMsg << std::endl ;
    return (-1) ;
  }
  /*
    And invoke it.
  */
  int32_t res = plugMgr.initializePlugin(initFunc) ;
  /*
    And can we invoke createObject? Did it work?
  */
  DummyAdapter dummy ;
  ProbMgmtAPI *clp =
    static_cast<ProbMgmtAPI*>(plugMgr.createObject("ProbMgmt",dummy)) ;
  if (clp == nullptr) {
    std::cout
      << "Apparent failure to create a ProbMgmt object." << std::endl ;
  } else {
  /*
    See if we can invoke a nontrivial method.
  */
    clp->readMps("exmip1.mps",true) ;
  }
  /*
    Shut down the plugin manager.
  */
  std::cout
    << "Shutting down plugins (executing exit functions)." << std::endl ;
  plugMgr.shutdown() ;
  /*
    Close the shim library.
  */
  std::cout << "Unloading libOsi2ClpShim." << std::endl ;
  delete clpShim ;

  std::cout << "END UNIT TEST" << std::endl ;
  return (0) ;

}
