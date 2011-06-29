#ifndef OSI2_PLUGIN_BASE
#define OSI2_PLUGIN_BASE

#include "Osi2StreamingException.hpp"

#define THROW throw StreamingException(__FILE__, __LINE__) \

#define CHECK(condition) if (!(condition)) \
  THROW << "CHECK FAILED: '" << #condition << "'"

#ifdef _DEBUG
  #define ASSERT(condition) if (!(condition)) \
     THROW << "ASSERT FAILED: '" << #condition << "'"
#else
  #define ASSERT(condition) {}
#endif // DEBUG


//----------------------------------------------------------------------

namespace base
{
  std::string getErrorMessage();
}

#endif // OSI2_PLUGIN_BASE


