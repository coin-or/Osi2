#ifndef DYNAMIC_LIBRARY_H
#define DYNAMIC_LIBRARY_H

#include <string>

/*! \brief

  A class to manage dynamic library load and unload.
*/
class Osi2DynamicLibrary
{

public:

  /*! \brief Load a plugin

    This method will load the plugin specified in \p path. If successful, it
    will return a pointer to an Osi2DynamicLibrary object for the plugin. If
    unsuccessful, it will return null and \p errorString will be loaded with an
    error message.
  */
  static Osi2DynamicLibrary *load(const std::string &path, 
				  std::string &errorString) ;

  /* \brief Load a symbol

    Load the specified symbol. The return value must be cast to the appropriate
    type for use.
  */
  void *getSymbol(const std::string &name) ;

  /// Get the default plugin directory
  inline std::string getDfltPluginDir() { return (dfltPluginDir_) ; }
  /// Set the default plugin directory
  inline void setDfltPluginDir(std::string dfltDir)
  { dfltPluginDir_ = dfltDir ; }

  /// Destructor
  ~Osi2DynamicLibrary() ;

private:

  /*! \name Constructors
      \brief Constructors

    Constructors are private because there should be exactly one instance of a
    DynamicLibrary object associated with a successfully loaded library.
  */
  //@{
  /// Default constructor
  Osi2DynamicLibrary() ;
  /*! \brief Constructor from dynamic library handle

    The parameter is be the platform-specific dynamic library handle.
  */
  Osi2DynamicLibrary(void *handle) ;
  /// Copy constructor
  Osi2DynamicLibrary(const Osi2DynamicLibrary &dynlib) ;
  //@}
  
  /// Platform-specific dynamic library handle.
  void *handle_;  

  /// Default plugin directory
  std::string dfltPluginDir_ ;
} ;

#endif
