/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  Based on original design and code by Gigi Sayfan published in five parts
  in Dr. Dobbs, starting November 2007.
*/
/*! \file Osi2DynamicLibrary.cpp

  This file contains the methods that handle loading and unloading of
  libraries.

  Currently works only for Linux & Solaris. There are hooks for Windows but
  they are untested and/or disabled.  -- lh, 110825 --
*/

#ifdef WIN32
  #include <Windows.h>
#else
  #include <dlfcn.h>
#endif

#include "Osi2DynamicLibrary.hpp"
#include <sstream>
#include <iostream>

/*
  Default for OSI2PLUGINDIR is $(libdir) (library installation directory).
  Specified with -D in the Makefile.
*/
#ifndef OSI2PLUGINDIR
# define OSI2DFLTPLUGINDIR "/usr/local/lib"
#else
# define OSI2DFLTPLUGINDIR OSI2PLUGINDIR
#endif

namespace Osi2 {

DynamicLibrary::DynamicLibrary (void *handle)
  : handle_(handle)
{ 
  dfltPluginDir_ = std::string(OSI2DFLTPLUGINDIR) ;
}

DynamicLibrary::~DynamicLibrary ()
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

DynamicLibrary *DynamicLibrary::load (const std::string &name, 
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

  return new DynamicLibrary(handle) ;
}

void *DynamicLibrary::getSymbol(const std::string &symbol)
{
  if (!handle_) return (0) ;
  
  #ifdef WIN32
    return ::GetProcAddress((HMODULE)handle_,symbol.c_str()) ;
  #else
    return ::dlsym(handle_,symbol.c_str()) ;
  #endif
}

}  // end namespace Osi2

