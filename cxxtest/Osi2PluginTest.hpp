
#ifndef OSI2PLUGINTEST_HPP_
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

/*
  Yes, it's a hack. This is the only place where it can be forced into the
  generated driver in the proper position.
*/
using namespace Osi2 ;

#endif /*OSI2PLUGINTEST_HPP_*/
