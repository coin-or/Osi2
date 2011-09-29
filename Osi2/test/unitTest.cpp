
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

  int retval = plugMgr.loadOneLib("libOsi2ClpShim.so.0") ;

  if (retval != 0) {
    std::cout
      << "Apparent failure opening " << clpShimPath << "." << std::endl ;
    std::cout
      << "Error code is " << retval << "." << std::endl ;
    return (-1) ;
  }

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
    delete clp ;
  }
  /*
    Shut down the plugin manager. This will call the plugin library exit
    functions and unload the libraries.
  */
  std::cout
    << "Shutting down plugins (executing exit functions)." << std::endl ;
  plugMgr.shutdown() ;

  std::cout << "END UNIT TEST" << std::endl ;
  return (0) ;

}
