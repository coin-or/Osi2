/*
  Copyright 2011 Lou Hafer, Matt Saltzman
  This code is licensed under the terms of the Eclipse Public License (EPL)
*/
/*! \file Osi2CtrlAPIMessages.hpp
    \brief Type definitions for Osi2::ControlAPI messages.
*/

#ifndef Osi2CtrlAPIMessages_HPP
# define Osi2CtrlAPIMessages_HPP

#include "CoinMessageHandler.hpp"

namespace Osi2 {

/*! \brief Enum to specify individual Osi2::ControlAPI messages

  The only purpose here is to define symbolic names for the messages. This
  makes it easy to add and remove messages.
*/

enum CtrlAPIMsg {
    CTRLAPI_INIT,
    CTRLAPI_LOADOK,
    CTRLAPI_UNREG,
    CTRLAPI_NOPLUGMGR,
    CTRLAPI_DUMMY_END
};

/*! \brief Osi2::ControlAPI messages

  This class holds the array of messages once they are loaded and provides the
  methods to use them. Its only reason for existence is the custom constructor,
  which loads the messages in the order defined by CtrlAPIMsg. All else is
  inherited from CoinMessages.

  See CoinMessageHandler for usage.
*/
class CtrlAPIMessages : public CoinMessages {

public:

    /// Custom constructor
    CtrlAPIMessages(Language language = us_en) ;

} ;

}  // end namespace Osi2

#endif

