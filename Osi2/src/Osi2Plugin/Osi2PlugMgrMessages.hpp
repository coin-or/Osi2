/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)
*/
/*! \file Osi2PluginMessage.hpp
    \brief Type definitions for Osi2 plugin manager messages
*/

#ifndef Osi2PlugMgrMessages_HPP
# define Osi2PlugMgrMessages_HPP

#include "CoinMessageHandler.hpp"

namespace Osi2 {

/*! \brief Enum to specify individual Osi2 plugin manager messages

  The only purpose here is to define symbolic names for the messages. This
  makes it easy to add and remove messages.
*/

enum PlugMgrMsg {
    PLUGMGR_INIT,
    PLUGMGR_LIBLDOK,
    PLUGMGR_LIBINITOK,
    PLUGMGR_LIBEXITOK,
    PLUGMGR_LIBCLOSE,
    PLUGMGR_REGAPIOK,
    PLUGMGR_LIBLDDUP,
    PLUGMGR_LIBNOTFOUND,
    PLUGMGR_LIBLDFAIL,
    PLUGMGR_LIBINITFAIL,
    PLUGMGR_LIBEXITFAIL,
    PLUGMGR_SYMLDFAIL,
    PLUGMGR_NOINITFUNC,
    PLUGMGR_REGDUPAPI,
    PLUGMGR_BADAPIPARM,
    PLUGMGR_BADVER,
    PLUGMGR_DUMMY_END
};

/*! \brief Osi2 plugin manager messages

  This class holds the array of messages once they loaded and provides the
  methods to use them. Its only reason for existence is the custom constructor,
  which loads the messages in the order defined by PlugMgrMsg. All else is
  inherited from CoinMessages.

  See CoinMessageHandler for usage.
*/
class PlugMgrMessages : public CoinMessages {

public:

    /// Custom constructor
    PlugMgrMessages(Language language = us_en) ;

} ;

}  // end namespace Osi2

#endif

