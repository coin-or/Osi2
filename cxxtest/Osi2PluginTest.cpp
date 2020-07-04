/*
  PluginManager tests
*/
#include <cxxtest/TestSuite.h>
#include <string>
#include <cstring>
#include <ostream>
#include "CoinHelperFunctions.hpp"
#include "Osi2PluginTest.hpp"
#include "Osi2ObjectAdapter.hpp"
#include "Osi2API.hpp"
#include "Osi2ClpLite_Wrap.hpp"
#include "Osi2ClpSimplexAPI.hpp"

static PluginManager &plugMgr = PluginManager::getInstance();

void Osi2PluginTest::setUp()
{
/*
  Add the pre-installation location for plugin libraries so that the tests
  will work before Osi2 is installed. Only add it once.
*/
  std::string uninstDir = "../src/Osi2Shims/.libs" ;
  std::string dfltDirs = plugMgr.getPluginDirsStr() ;
  if (dfltDirs.find(uninstDir) == std::string::npos) {
    dfltDirs = dfltDirs+':'+uninstDir ;
    plugMgr.setPluginDirsStr(dfltDirs) ;
  }
}

void Osi2PluginTest::test_loadOneLib()
{
  TS_TRACE("Test loadOneLib()") ;

  std::vector<std::string> dfltDirArray = plugMgr.getPluginDirs() ;
  std::string dfltDir = dfltDirArray[0] ;
  TS_ASSERT(strcmp(dfltDir.c_str(), OSI2PLUGINDIR) == 0) ;

  std::string libName = "libOsi2ClpShim.so" ;
  int retval = plugMgr.loadOneLib(libName);
  TS_ASSERT(!retval);
}

void Osi2PluginTest::test_createObject()
{
  TS_TRACE("Test createObject()");

  DummyAdapter dummy;
  PluginUniqueID libID = 0;
  ClpLite_Wrap *clpWrap = static_cast<ClpLite_Wrap *>
      (plugMgr.createObject("ClpSimplex", libID, dummy));
  TS_ASSERT_DIFFERS(clpWrap, nullptr);
  ClpSimplexAPI *clp = static_cast<ClpSimplexAPI *>
    (clpWrap->getAPIPtr("ClpSimplex")) ;
  TS_ASSERT_DIFFERS(clp, nullptr) ;
  int retval = clp->readMps("exmip1.mps", true) ;
  // Expecting failure here, exmip1 not found, retval = -1
  TS_ASSERT_LESS_THAN(retval,0) ;
  retval = plugMgr.destroyObject("ClpSimplex", 0, clpWrap);
  TS_ASSERT_EQUALS(0, retval);
}

