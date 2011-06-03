#include <konig/ai/bdefenceai.hpp>

#include <konig/ai/negative_defensive_lead.hpp>
#include <konig/ai/negative_defensive_follow.hpp>

namespace konig { namespace ai {

void BDefenceAi::reset(FateAi const&)
{
  tried_low_trump_ = false;
}

Card BDefenceAi::play_card(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();

  // If there's only one legal play, play it
  if (plays.size() == 1) {
    return *plays.begin();
  }

  // If declarer has already taken a trick, then it doesn't matter; play
  // anything legal
  if (ai.num_tricks_taken_by(ai.declarer()) > 0) {
    return *plays.begin();
  }

  // Check to see whether all legal plays are equivalent; if so play whatever.
  if (ai.cards_are_equivalent(plays)) {
    return *plays.begin();
  }

  if (trick.leader() == ai.position()) {
    // I am leading to the trick
    return negative_defensive_lead(ai, tried_low_trump_);
  } else {
    // I am following to the trick
    return negative_defensive_follow(ai);
  }

  KONIG_FATAL("not implemented\ntrick " << trick << " hand " << hand);
}

}}

