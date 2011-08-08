#ifndef OSI2OBJECT_ADAPTER_H
#define OSI2OBJECT_ADAPTER_H

#include "Osi2Plugin.hpp"

namespace Osi2 {

/*! \brief This interface is used to adapt C plugin objects to C++ plugin
	   objects.

  It must be passed to the PluginManager::createObject() function.
*/
struct IObjectAdapter
{
  virtual ~IObjectAdapter() {}
  virtual void *adapt(void *object, DestroyFunc df) = 0 ;
} ;

/*! \brief This template should be used if the object model implements the
	   dual C/C++ object design pattern.

  Otherwise you need to provide your own object adapter class that implements
  IObjectAdapter
*/
template <typename T, typename U>
struct ObjectAdapter : public IObjectAdapter
{
  virtual void *adapt (void *object, DestroyFunc df)
  {
    return new T((U *)object, df);
  }
} ;

struct DummyAdapter : public IObjectAdapter
{
  virtual void *adapt(void *object, DestroyFunc df)
  {
    return (static_cast<void*>(0)) ;
  }
} ;

}  // end namespace Osi2

#endif // OSI2OBJECT_ADAPTER_H
