#include <konig/ai/tai.hpp>

namespace konig { namespace ai {

void TAi::reset(FateAi const&)
{
}

std::vector<Announcement> TAi::announce(FateAi const&)
{
  return std::vector<Announcement>();
}

Card TAi::play_card(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();

  KONIG_FATAL("not implemented\ntrick " << trick << " hand " << hand);
}

}}

