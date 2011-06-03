#include <konig/ai/pdefenceai.hpp>

#include <boost/range/algorithm/max_element.hpp>

#include <konig/ai/negative_defensive_lead.hpp>
#include <konig/ai/negative_defensive_follow.hpp>

namespace konig { namespace ai {

void PDefenceAi::reset(FateAi const&)
{
  tried_low_trump_ = false;
}

Card PDefenceAi::play_card(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();

  // If there's only one legal play, play it
  if (plays.size() == 1) {
    return *plays.begin();
  }

  auto const tricks_by_declarer = ai.num_tricks_taken_by(ai.declarer());

  // If declarer has already taken two tricks, then it doesn't matter; play
  // anything legal
  if (tricks_by_declarer > 1) {
    return *plays.begin();
  }

  // Check to see whether all legal plays are equivalent; if so play whatever.
  if (ai.cards_are_equivalent(plays)) {
    return *plays.begin();
  }

  if (trick.leader() == ai.position()) {
    // I am leading to the trick
    if (tricks_by_declarer == 0) {
      // Declarer has taken no tricks.  If he might hold the Skus, try to flush
      // that out
      if (ai.fates_of(Card(TrumpRank::skus)).count(
          CardFate::held_by(ai.declarer())
        )) {
        auto const potential_trump = boost::prior(plays.end());
        if (potential_trump->trump()) {
          return *potential_trump;
        }

        // No trump to lead; try a long suit instead?
        std::array<size_t, Suit::trumps> suit_lengths;
        for (Suit s=Suit::min; s != Suit::trumps; ++s) {
          suit_lengths[s] = hand.count(s);
        }
        auto longest_it = boost::range::max_element(suit_lengths);
        Suit longest(Suit::internal_enum(longest_it-suit_lengths.begin()));
        assert(plays.count(longest));
        return *plays.lower_bound(longest);
      } else {
        // Declarer cannot hold the Skus.  Guess he's trying a two-kings
        // Picollo, so lead a small trump
        auto const potential_trump = plays.lower_bound(Suit::trumps);
        if (potential_trump != plays.end()) {
          return *potential_trump;
        }

        // We no longer have a trump to lead, so ordinary negative defensive
        // play will do
        return negative_defensive_lead(ai, tried_low_trump_);
      }
    } else {
      // Declarer has taken a trick, so we can play simply negatively.
      return negative_defensive_lead(ai, tried_low_trump_);
    }
  } else {
    // I am following to the trick
    // For now we do the lazy thing and always play negatively
    // TODO: at least sometimes (e.g. in last few tricks if declarer hasn't won
    // yet) we should play positively
    return negative_defensive_follow(ai);
  }

  KONIG_FATAL("not implemented\ntrick " << trick << " hand " << hand);
}

}}

