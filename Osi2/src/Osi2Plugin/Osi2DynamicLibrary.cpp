#ifdef WIN32
  #include <Windows.h>
#else
  #include <dlfcn.h>
#endif

#include "Osi2DynamicLibrary.hpp"
#include <sstream>
#include <iostream>

Osi2DynamicLibrary::Osi2DynamicLibrary (void *handle)
  : handle_(handle)
{ }

Osi2DynamicLibrary::~Osi2DynamicLibrary ()
{
  if (handle_)
  {
  #ifndef WIN32
    ::dlclose(handle_) ;
  #else
    ::FreeLibrary((HMODULE)handle_) ;
  #endif
  }
}

Osi2DynamicLibrary *Osi2DynamicLibrary::load (const std::string &name, 
					      std::string &errorString)
{
  if (name.empty()) {
    errorString = "Empty path." ;
    return (0) ;
  }
  
  void *handle = NULL ;

# ifdef WIN32
  handle = ::LoadLibraryA(name.c_str()) ;
  if (handle == NULL) {
    DWORD errorCode = ::GetLastError() ;
    std::stringstream ss ;
    ss << "LoadLibrary(" << name << ") Failed. errorCode: " << errorCode ; 
    errorString = ss.str() ;
  }
# else
  handle = ::dlopen(name.c_str(),RTLD_NOW) ;
  if (!handle) {
    std::string dlErrorString ;
    const char *zErrorString = ::dlerror() ;
    if (zErrorString)
      dlErrorString = zErrorString ;
    errorString += "Failed to load \"" + name + '"' ;
    if (dlErrorString.size())
      errorString += ": " + dlErrorString ;
    return NULL ;
  }
# endif

  return new Osi2DynamicLibrary(handle) ;
}

void *Osi2DynamicLibrary::getSymbol(const std::string &symbol)
{
  if (!handle_) return (0) ;
  
  #ifdef WIN32
    return ::GetProcAddress((HMODULE)handle_,symbol.c_str()) ;
  #else
    return ::dlsym(handle_,symbol.c_str()) ;
  #endif
}

