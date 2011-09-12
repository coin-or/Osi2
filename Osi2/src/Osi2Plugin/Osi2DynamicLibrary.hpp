#ifndef DYNAMIC_LIBRARY_H
#define DYNAMIC_LIBRARY_H

/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  Based on original design and code by Gigi Sayfan published in five parts
  in Dr. Dobbs, starting November 2007.
*/
/*! \file Osi2DynamicLibrary.hpp

  Declarations for the underlying dynamic library and symbol management
  methods.
*/

#include <string>

namespace Osi2 {

/*! \brief A class to manage dynamic library load and unload.

  A call to the static method #load will load a library and return
  a DynamicLibrary object. The #getSymbol method can then be used to
  load individual methods from the library.

  \todo Restore/check Windows functionality.

  \todo Convert/augment with libltdl.
*/
/*
  Doxygen's builtin notion of grouping gets in the way here. What you see is a
  compromise.  -- lh, 110912 --
*/
class DynamicLibrary
{

public:

/*! \name Library Management
    \brief Library management methods
*/
//@{

  /*! \brief Load a plugin

    This method will load the plugin specified in \p path. If successful, it
    will return a pointer to an DynamicLibrary object for the plugin. If
    unsuccessful, it will return null and \p errStr will be loaded with an
    error message.
  */
  static DynamicLibrary *load(const std::string &path, std::string &errStr) ;
//@}

/*! \name Symbol Management
    \brief Symbol management methods
*/
//@{
  /* \brief Load a symbol

    Load the specified symbol. The return value must be cast to the appropriate
    type for use. If unsuccessful, it will return null and \p errStr will be
    loaded with an error message.
  */
  void *getSymbol(const std::string &name, std::string &errStr) ;
//@}

/*! \name Miscellaneous
    \brief Miscellaneous options
*/
//@{
  /// Get the default plugin directory
  inline std::string getDfltPluginDir() { return (dfltPluginDir_) ; }
  /// Set the default plugin directory
  inline void setDfltPluginDir(std::string dfltDir)
  { dfltPluginDir_ = dfltDir ; }
//@}

/*! \name Destructor */
//@{
  /*! \brief Destructor

    Unloads the library as part of execution.
  */
  ~DynamicLibrary() ;
//@}

private:

/*! \name Constructors
    \brief Constructors

  Constructors are private because there should be exactly one instance of a
  DynamicLibrary object associated with a successfully loaded library. This
  object is returned by #load.
*/
//@{
  /// Default constructor
  DynamicLibrary() ;

  /*! \brief Constructor from dynamic library handle

    The parameter is be the platform-specific dynamic library handle.
  */
  DynamicLibrary(void *handle) ;

  /// Copy constructor
  DynamicLibrary(const DynamicLibrary &dynlib) ;
//@}
  
  /// Platform-specific dynamic library handle.
  void *handle_;  

  /// Full path of associated library
  std::string fullPath_ ;

  /// Default plugin directory
  std::string dfltPluginDir_ ;
} ;

}  // end namespace Osi2

#endif
