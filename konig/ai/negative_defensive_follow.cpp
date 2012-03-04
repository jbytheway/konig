#include <konig/ai/negative_defensive_follow.hpp>

namespace konig { namespace ai {

Card negative_defensive_follow(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();

  Suit s = trick.suit();

  bool const will_rise =
    (plays.begin()->trump() && s != Suit::trumps) ||
    (plays.begin()->suit() == s && *plays.begin() > trick.winning_card());

  // Save the biggest thing we can play
  auto best_to_get_rid_of = boost::prior(plays.end());
  if (!best_to_get_rid_of->trump()) {
    // If we're not playing trumps we want the biggest rank
    best_to_get_rid_of =
      std::max_element(plays.begin(), plays.end(), Card::CompareSuitRanks());
  }

  if (!will_rise) {
    // I'm not going to win, so I should discard the most dangerous card I
    // can
    return *best_to_get_rid_of;
  }

  // If we have a chance of winning then we must be following suit or
  // roughing, which means that all the cards are the same suit
  assert(plays.begin()->suit() == boost::prior(plays.end())->suit());

  // If I'm roughing the first round then probably noone else is so I should
  // play big
  if (plays.begin()->suit() == Suit::trumps && s != Suit::trumps &&
    !ai.had_first_round(s)) {
    return *best_to_get_rid_of;
  }

  // If I am before declarer then I want to play the smallest thing I can
  bool const declarer_yet_to_play =
    trick.played() < ai.seats_after_declarer();
  if (declarer_yet_to_play) {
    return *plays.begin();
  }

  // At this point we know that the current trick is doomed; it's a question
  // of whether I want the lead or not.
  // TODO: figure out whether we want to win or not.
  // Typically, we do want the earlier player to win, so as to lead up to
  // declarer next time, so play high.
  return *best_to_get_rid_of;
}

}}

