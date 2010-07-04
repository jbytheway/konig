#include <konig/ai/rpartnerai.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace ai {

void RPartnerAi::reset(FateAi const&)
{
}

std::vector<Announcement> RPartnerAi::announce(FateAi const&)
{
  return std::vector<Announcement>();
}

Card RPartnerAi::play_card(FateAi const&)
{
  KONIG_FATAL("no implemented");
}

}}

