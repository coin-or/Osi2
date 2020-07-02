/*
  Copyright 2019 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)
*/
/*! \file Osi2ParamMgmtAPIMessages.hpp
    \brief Type definitions for Osi2::ParamMgmtAPI messages.
*/

#ifndef Osi2ParamMgmtAPIMessages_HPP
# define Osi2ParamMgmtAPIMessages_HPP

#include "CoinMessageHandler.hpp"

namespace Osi2 {

/*! \brief Enum to specify individual Osi2::ParamMgmtAPI messages

  The only purpose here is to define symbolic names for the messages. This
  makes it easy to add and remove messages.
*/

enum ParamMgmtAPIMsg {
    PMMGAPI_INIT,
    PMMGAPI_NOSUPPORT,
    PMMGAPI_DUPIDENT,
    PMMGAPI_UNREG,
    PMMGAPI_PARMUNREG,
    PMMGAPI_OPFAIL,
    PMMGAPI_NOPLUGMGR,
    PMMGAPI_DUMMY_END
};

/*! \brief Osi2::ParamMgmtAPI messages

  This class holds the array of messages once they are loaded and provides the
  methods to use them. Its only reason for existence is the custom constructor,
  which loads the messages in the order defined by ParamMgmtAPIMsg. All else is
  inherited from CoinMessages.

  See CoinMessageHandler for usage.
*/
class ParamMgmtAPIMessages : public CoinMessages {

public:

    /// Custom constructor
    ParamMgmtAPIMessages(Language language = us_en) ;

} ;

}  // end namespace Osi2

#endif

