
#ifndef OSI2LUGINTEST_HPP_
#define OSI2PLUGINTEST_HPP_

#include <cxxtest/TestSuite.h>

#include "Osi2PluginManager.hpp"

namespace Osi2
{

class Osi2PluginTest : public CxxTest::TestSuite
{
public:
//	void setUp(void);

	void test_loadOneLib(void);

	void test_createObject();

//	void tearDown(void);
};
}
#endif /*OSI2PLUGINTEST_HPP_*/
