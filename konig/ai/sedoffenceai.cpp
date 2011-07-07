#include <konig/ai/sedoffenceai.hpp>

namespace konig { namespace ai {

SedOffenceAi::SedOffenceAi() :
  OffenceAi(
    _king_rippiness_penalty = 10,
    _trumps_for_unannounced_bird = std::vector<OffenceAi::Count>{5, 6, 7}
  )
{}

}}

