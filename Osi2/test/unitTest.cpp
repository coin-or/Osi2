
#include "CoinMessageHandler.hpp" 
#include "CoinPackedMatrix.hpp"
#include "ClpSimplex.hpp"

#include "Osi2PluginManager.hpp"
#include "Osi2DynamicLibrary.hpp"
#include "Osi2ObjectAdapter.hpp"
  
int main(int argC, char* argV[])
{

  std::cout << "START UNIT TEST" << std::endl ;

  std::cout << "Instantiating PluginManager." << std::endl ;

  Osi2PluginManager &plugMgr = Osi2PluginManager::getInstance() ;

  /*
    Now let's try to load the shim. Hardwire the path for testing.
  */
  std::string clpShimPath =
      "/cs/mitacs4/Osi2/Coin-Osi2-SunX86/lib/libOsi2ClpShim.so.0" ;
  std::string errMsg ;
  Osi2DynamicLibrary *clpShim = Osi2DynamicLibrary::load(clpShimPath,errMsg) ;
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
  Osi2_InitFunc initFunc =
    reinterpret_cast<Osi2_InitFunc>(clpShim->getSymbol("initPlugin")) ;
  if (initFunc == 0) {
    std::cout
      << "Apparent failure to obtain the init method." << std::endl ;
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

  std::cout << "END UNIT TEST" << std::endl ;

  return (0) ;

}
