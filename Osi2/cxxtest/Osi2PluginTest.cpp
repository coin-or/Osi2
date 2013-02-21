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
#include "Osi2ProbMgmtAPI.hpp"

using namespace Osi2;

static PluginManager &plugMgr = PluginManager::getInstance();

//void setUp(void)
//{
//	// TODO: Implement setUp() function.
//}

void Osi2PluginTest::test_loadOneLib(void)
{
	/*
	 * Test that th
	 */

	std::cout << std::endl << "Plugin directory: " << OSI2PLUGINDIR << std::endl;
	TS_TRACE("Test loadOneLib()");
	std::string libName = "libOsi2ClpShim.so";
	std::string dfltDir = plugMgr.getDfltPluginDir();
	TS_ASSERT(strcmp(dfltDir.c_str(), OSI2PLUGINDIR) == 0);

	char dirSep = CoinFindDirSeparator();
	std::string shimPath = dfltDir + dirSep + libName;

	int retval = plugMgr.loadOneLib(libName);
	TS_ASSERT(!retval);
}

void Osi2PluginTest::test_createObject(void)
{
	TS_TRACE("Test createObject()");
	DummyAdapter dummy;
	PluginUniqueID libID = 0;
	ProbMgmtAPI *clp =
			static_cast<ProbMgmtAPI *>(plugMgr.createObject("ProbMgmt", libID, dummy));
	TS_ASSERT_DIFFERS(clp, nullptr);
    clp->readMps("exmip1.mps", true);
    int retval = plugMgr.destroyObject("ProbMgmt", 0, clp);
    TS_ASSERT_LESS_THAN_EQUALS(0, retval);
}

//void tearDown(void)
//{
//	// TODO: Implement tearDown() function.
//}
