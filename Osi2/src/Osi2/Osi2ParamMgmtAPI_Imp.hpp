/*
  Copyright 2019 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)

  $Id$
*/
/*! \file Osi2ParamMgmtAPI_Imp.hpp

  Defines an implementation of Osi2::ParamMgmtAPI, an API to provide some
  uniformity and syntactic sugar for parameter management.
*/


#ifndef Osi2ParamMgmtAPI_Imp_HPP
# define Osi2ParamMgmtAPI_Imp_HPP

#include "Osi2PluginManager.hpp"

#include "Osi2ParamMgmtAPI.hpp"
#include "Osi2ParamMgmtAPIMessages.hpp"

namespace Osi2 {

/*! \brief Osi2 ParamMgmtAPI implementation

  This class implements the Osi2::ParamMgmt API.
*/
class ParamMgmtAPI_Imp : public ParamMgmtAPI {

public:

/*! \name Constructors and Destructors */
//@{
  /// Default constructor
  ParamMgmtAPI_Imp() ;
  /// Registration constructor
  ParamMgmtAPI_Imp(std::string name) ;
  /// Default copy constructor
  ParamMgmtAPI_Imp(const ParamMgmtAPI_Imp &original) ;
  /// Assignment
  ParamMgmtAPI_Imp &operator=(const ParamMgmtAPI_Imp &rhs) ;

  /// Create (factory constructor)
  ParamMgmtAPI *create() ;
  /// Clone (factory copy constructor)
  ParamMgmtAPI *clone() ;

  /// Destructor
  ~ParamMgmtAPI_Imp() ;
//@}

/*! \name ParamMgmt API control methods

  Miscellaneous methods that control the behaviour of a ParamMgmtAPI object.
*/
//@{

  /// Set the log (verbosity) level
  inline void setLogLvl(int logLvl) {
    logLvl_ = logLvl ;
    msgHandler_->setLogLevel(logLvl_) ;
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
  inline CoinMessageHandler *getMsgHandler() const {
      return (msgHandler_) ;
  }

  /// Report owner of message handler (false if owned by client).
  inline bool dfltHandler () const {
      return (dfltHandler_) ;
  }

  //@}

private:

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
