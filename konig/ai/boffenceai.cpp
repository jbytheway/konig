#include <konig/ai/boffenceai.hpp>

#include <konig/ai/negative_offensive.hpp>

namespace konig { namespace ai {

void BOffenceAi::reset(FateAi const&)
{
}

Card BOffenceAi::play_card(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();

  // If there's only one legal play, play it
  if (plays.size() == 1) {
    return *plays.begin();
  }

  // If I've already taken a trick, then it doesn't matter; play anything legal
  if (ai.num_tricks_taken_by(ai.position()) > 0) {
    return *plays.begin();
  }

  return negative_offensive(ai);

  KONIG_FATAL("not implemented\ntrick " << trick << " hand " << hand);
}

}}

