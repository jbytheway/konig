#include <konig/ai/sdoffenceai.hpp>

namespace konig { namespace ai {

SdOffenceAi::SdOffenceAi() :
  OffenceAi(
    _king_rippiness_penalty = 10,
    _trumps_for_unannounced_bird = std::vector<OffenceAi::Count>{5, 6, 7},
    _min_desired_rippiness = 3
  )
{
}

}}

