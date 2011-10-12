/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/

#ifndef Osi2ControlAPI_Imp_HPP
# define Osi2ControlAPI_Imp_HPP

#include "Osi2PluginManager.hpp"

#include "Osi2ControlAPI.hpp"
#include "Osi2CtrlAPIMessages.hpp"

namespace Osi2 {

/*! \brief Osi2 ControlAPI implementation

  This class implements the Osi2 Control API (Osi2::ControlAPI).
*/
class ControlAPI_Imp : public ControlAPI {

public:

  /// \name Constructors and Destructors
  //@{
  /// Virtual constructor
  ControlAPI *create() ;
  /// Virtual copy constructor
  ControlAPI *clone() ;
  /// Destructor
  ~ControlAPI_Imp() ;
  /// Default constructor; \sa #create
  ControlAPI_Imp() ;
  /// Default copy constructor; \sa #clone
  ControlAPI_Imp(const ControlAPI_Imp &original) ;
  /// Assignment
  ControlAPI_Imp &operator=(const ControlAPI_Imp &rhs) ;
  //@}

  /*! \name Library load and unload
      \brief Methods to load and unload plugin libraries

    Given only a short name xxx the method will look for libOsi2XxxShim.so in
    the default plugin library directory. The other load methods allow for an
    arbitrary mapping between the short name and the plugin library.
  */
  //@{

  /*! \brief Load the specified plugin library

    Given \p shortName 'xxx', attempt to load libOsi2XxxShim.so from the
    default plugin library directory. Associate the plugin library with
    \p shortName.
  */
  virtual int load(const std::string &shortName) ;

  /*! \brief Load the specified plugin library

    Attempt to load library \p libName from the default plugin library
    directory. Associate the plugin library with \p shortName.
  */
  virtual int load(const std::string &shortName, const std::string &libName) ;

  /*! \brief Load the specified plugin library

    Attempt to load library \p libName from directory \p dirName. Associate
    the plugin library with \p shortName.
  */
  virtual int load(const std::string &shortName,
  		   const std::string &libName, const std::string *dirName) ;

  /*! \brief Unload the specified library.

    Unloads the specified library. The return value will be 0 if all went
    smoothly, non-zero on error.
  */
  virtual int unload(const std::string &shortName) ;

  //@}

  /*! \name Control API control methods

    Miscellaneous methods that control the behaviour of a ControlAPI object.
  */
  //@{

  /// Get the default plugin directory
  inline std::string getDfltPluginDir() const { return (dfltPluginDir_) ; }

  /// Set the default plugin directory
  inline void setDfltPluginDir(std::string dfltDir)
  { dfltPluginDir_ = dfltDir ; }


  /// Set the log (verbosity) level
  inline void setLogLvl(int logLvl) {
    logLvl_ = logLvl ; msgHandler_->setLogLevel(logLvl_) ;
  }

  /// Get the log (verbosity) level
  inline int getLogLvl() const { return (logLvl_) ; }

  /*! \brief Set the message handler

    Replaces the current message handler. If newHandler is null, the existing
    handler is replaced with a default handler. It is the responsibility of
    the client to destroy any handler it supplies. The plugin manager takes
    responsibility for a default handler.
  */
  void setMsgHandler(CoinMessageHandler *newHandler) ;

  /// Get the message handler
  inline CoinMessageHandler *getMsgHandler() const { return (msgHandler_) ; }

  /// Report owner of message handler (false if owned by client).
  inline bool dfltHandler () const { return (dfltHandler_) ; }

  //@}

private:

  /// Utility method to find the plugin manager and cache a reference.
  PluginManager *findPluginMgr() ;
  /// Cached reference to plugin manager.
  PluginManager *pluginMgr_ ;

  /// Map type for knownLibMap_
  typedef std::map<std::string,std::string> LibMapType ;
  /// Map to associate short names with full paths for plugin libraries
  LibMapType knownLibMap_ ;

  /// Default plugin library directory
  std::string dfltPluginDir_ ;

  /// Indicator; false if the message handler belongs to the client
  bool dfltHandler_ ;
  /// Message handler
  CoinMessageHandler *msgHandler_ ;
  /// Messages
  CoinMessages msgs_ ;
  /// Log (verbosity) level
  int logLvl_ ;


} ;

} // namespace Osi2 ;

#endif
