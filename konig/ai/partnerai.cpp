#include <konig/ai/partnerai.hpp>

namespace konig { namespace ai {

PartnerAi::PartnerAi() :
  OffenceAi(
    _king_rippiness_penalty = 1,
    _trumps_for_unannounced_bird = std::vector<OffenceAi::Count>{8, 8, 8},
    _min_desired_rippiness = 4
  )
{}

}}

