#include <konig/ai/rdefenceai.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace ai {

void RDefenceAi::reset(FateAi const&)
{
}

std::vector<Announcement> RDefenceAi::announce(FateAi const&)
{
  return std::vector<Announcement>();
}

Card RDefenceAi::play_card(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  KONIG_FATAL("not implemented " << trick);
}

}}

