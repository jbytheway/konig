#include <konig/ai/roffenceai.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace ai {

std::vector<Announcement> ROffenceAi::announce(FateAi const&)
{
  return std::vector<Announcement>();
}

void ROffenceAi::play_start(FateAi const&)
{
}

Card ROffenceAi::play_card(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  //Cards const& hand = ai.hand();
  //Cards const plays = ai.legal_plays();

  KONIG_FATAL("not implemented " << trick);
}

}}

