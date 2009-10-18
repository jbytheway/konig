#ifndef KONIG__MESSAGETYPE_HPP
#define KONIG__MESSAGETYPE_HPP

#include <konig/core.hpp>
#include <konig/utility/enum.hpp>

namespace konig {

#define KONIG_MESSAGETYPE_VALUES() \
  (joined)(rejection)(getSetting)(setSetting)(notifySetting)\
  (startGame)

KONIG_UTILITY_ENUM_HEADER(MessageType, KONIG_MESSAGETYPE_VALUES())

}

#endif // KONIG__MESSAGETYPE_HPP

