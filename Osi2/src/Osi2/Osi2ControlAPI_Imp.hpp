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

  /*! \name API load and unload
      \brief Methods to load and unload objects implementing specific APIs.

      The general paradigm is that specific APIs are designated by names
      (strings), as are solvers.
  */
  //@{

  /*! \brief Load the specified API.

    A call to load a specific API may simply return a reference to an existing
    object implementing the API, or it may trigger the dynamic loading of a
    plugin which can supply an object implementing the specified API. A request
    for a specific solver is handled in the same manner: If the specified
    solver is already loaded, it will be used, otherwise it will be loaded.

    The load method returns the requested object as a generic API*; this can be
    cast to the requested API with dynamic_cast. This provides a semblance of
    type safety as the dynamic cast will fail if the client attempts to cast it
    to something other than what was requested.

    The method will return nullptr on failure; more detailed information may
    be available in \p rtncode. \p rtncode will be 0 if the call is
    successful.
  */
  API *load(std::string api, std::string solver, int &rtncode) ;

  /*! \brief Unload the specified API.

    Unloads the specified API. The return value will be 0 if all went smoothly,
    non-zero on error.
  */
  int unload(API *api) ;

  //@}

  /*! \name Control API control methods

    Miscellaneous methods that control the behaviour of a ControlAPI object.
  */
  //@{

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
