#include <konig/ai/partnerai.hpp>

namespace konig { namespace ai {

PartnerAi::PartnerAi() :
  OffenceAi(
    _trumps_for_unannounced_bird = std::vector<OffenceAi::Count>{8, 8, 8}
  )
{}

}}

