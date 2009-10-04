#ifndef KONIG__MESSAGETYPE_HPP
#define KONIG__MESSAGETYPE_HPP

#include <konig/core.hpp>
#include <konig/utility/enum.hpp>

namespace konig {

#define KONIG_MESSAGETYPE_VALUES() \
  (rejection)(getSetting)(setSetting)(notifySetting)

KONIG_UTILITY_ENUM_HEADER(MessageType, KONIG_MESSAGETYPE_VALUES())

}

#endif // KONIG__MESSAGETYPE_HPP

