#ifndef KONIG_AI__CARDFATE_HPP
#define KONIG_AI__CARDFATE_HPP

#include <konig/utility/enum.hpp>

namespace konig { namespace ai {

#define KONIG_AI_CARDFATE_VALUES() \
  (hand0)(hand1)(hand2)(hand3)\
  (talon)(reject)(discard)\
  (played)

KONIG_UTILITY_ENUM_HEADER(CardFate, KONIG_AI_CARDFATE_VALUES())

}}

#endif // KONIG_AI__CARDFATE_HPP

