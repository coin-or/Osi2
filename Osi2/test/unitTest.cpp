
#include "CoinMessageHandler.hpp" 
#include "CoinPackedMatrix.hpp"
#include "ClpSimplex.hpp"

#include "Osi2PluginManager.hpp"
#include "Osi2DynamicLibrary.hpp"
#include "Osi2ObjectAdapter.hpp"

using namespace Osi2 ;
  
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
    And can we invoke createObject?
  */
  DummyAdapter dummy ;
  ClpSimplex *clp =
    static_cast<ClpSimplex*>(plugMgr.createObject("ClpSimplex",dummy)) ;
  
  std::cout
    << "The zero tolerance is " << clp->zeroTolerance() << "." << std::endl ;

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
