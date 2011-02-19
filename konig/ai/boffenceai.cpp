#include <konig/ai/boffenceai.hpp>

#include <boost/range/algorithm/max_element.hpp>

#include <konig/ai/get_dangerous_suit_candidates.hpp>

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

  // If I am so foolish as to be holding the Skus then play that and put me out
  // of my misery
  if (plays.count(TrumpRank::skus)) {
    return Card(TrumpRank::skus);
  }

  if (trick.leader() == ai.position()) {
    // I am leading to the trick (note this only happens once, for the very
    // first trick)

    // This function picks out good plays in side suits
    auto candidates = get_dangerous_suit_candidates(ai);

    // We should also consider trumps
    Card const big_trump = *boost::prior(plays.end());
    if (big_trump.trump()) {
      // This makes the mond as bad as a singleton queen.
      int score = big_trump.trump_rank() - TrumpRank::mond + 7;
      candidates.push_back({score, big_trump});
    }

    // Having gathered all the candidates, play the best one
    if (!candidates.empty()) {
      auto it = boost::range::max_element(candidates);
      return it->second;
    }
  } else {
    // I am following
    Suit s = trick.suit();

    bool const will_rise =
      (plays.begin()->trump() && s != Suit::trumps) ||
      (plays.begin()->suit() == s && *plays.begin() > trick.winning_card());

    // Save the biggest thing we can play
    auto best_to_get_rid_of = boost::prior(plays.end());
    if (!best_to_get_rid_of->trump()) {
      // If we're not playing trumps we want the biggest rank
      best_to_get_rid_of =
        std::max_element(plays.begin(), plays.end(), Card::CompareRanks());
    }

    if (!will_rise) {
      // I'm not going to win, so I should discard the most dangerous card I
      // can
      return *best_to_get_rid_of;
    }

    // I might win, so rise minimally
    return *plays.begin();
  }

  KONIG_FATAL("not implemented\ntrick " << trick << " hand " << hand);
}

}}

