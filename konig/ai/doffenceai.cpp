#include <konig/ai/doffenceai.hpp>

namespace konig { namespace ai {

DOffenceAi::DOffenceAi() :
  OffenceAi(
    _king_rippiness_penalty = 10,
    _trumps_for_unannounced_bird = std::vector<OffenceAi::Count>{5, 6, 7}
  )
{}

}}

