#include <konig/ai/negative_defensive_lead.hpp>

#include <boost/range/empty.hpp>
#include <boost/range/algorithm/min_element.hpp>

namespace konig { namespace ai {

Card negative_defensive_lead(FateAi const& ai, bool& tried_low_trump)
{
  Trick const& trick = ai.tricks().back();
  Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();

  auto other_hands = ai.other_players_hands();
  auto other_defenders = other_hands;
  other_defenders.erase(CardFate::held_by(ai.declarer()));

  // Look for a king to underlead
  if (plays.count(SuitRank::king)) {
    for (Suit s = Suit::min; s != Suit::trumps; ++s) {
      if (ai.had_first_round(s)) continue;
      Card const king = Card(s, SuitRank::king);
      if (!plays.count(king)) continue;

      auto it = plays.upper_bound(s);
      SuitRank rank = SuitRank::king;
      while (it != plays.begin() &&
        boost::prior(it)->suit() == s) {
        if (boost::prior(it)->suit_rank() != rank) {
          // We've found a gap, therefore it's probably worth underleading
          // this king
          return *plays.lower_bound(s);
        }
        --it; --rank;
      }
    }
  }

  // No suitable king to underlead; on to other ideas
  bool const leading_round_to = ai.seats_after_declarer() == 1;
  bool const leading_through = ai.seats_after_declarer() == 3;
  if (leading_round_to) {
    // If we have a small trump, then try leading that round
    auto low_trump = plays.lower_bound(Suit::trumps);

    if (low_trump != plays.end() &&
      !tried_low_trump &&
      low_trump->trump_rank() < 5) {
      tried_low_trump = true;
      return *low_trump;
    }
  } else if (leading_through) {
    // This is a nasty poition to be in.  We want to play a middling card to
    // prevent declarer playing low and thus the later players beating him.
    for (Suit s = Suit::min; s != Suit::trumps; ++s) {
      if (ai.had_first_round(s)) continue;
      Card candidate = Card(s, SuitRank::knight);
      if (!plays.count(candidate)) {
        candidate = Card(s, SuitRank::jack);
        if (!plays.count(candidate)) continue;
      }

      return candidate;
    }

    // Otherwise, try to unblock a king or queen
    for (Suit s = Suit::min; s != Suit::trumps; ++s) {
      if (ai.had_first_round(s)) continue;
      Card candidate = Card(s, SuitRank::king);
      if (!plays.count(candidate)) {
        candidate = Card(s, SuitRank::queen);
        if (!plays.count(candidate)) continue;
      } else if (plays.count(Card(s, SuitRank::queen))) {
        // No use playing the king if we also hold the queen
        continue;
      }

      // Here candidate is a king or queen; we must also hold a small pip for
      // it to be worth playing
      auto small_pips = std::make_pair(
        plays.lower_bound(Card(s, SuitRank::seven)),
        plays.lower_bound(Card(s, SuitRank::ten))
      );
      if (!boost::empty(small_pips)) {
        return candidate;
      }
    }

    // Otherwise, play a middling trump to get the lead somewhere else
    auto trump = plays.end();
    do {
      --trump;
      if (!trump->trump()) break;
      if (!ai.guaranteed_to_win_against(*trump, other_defenders)) {
        return *trump;
      }
    } while (trump != plays.begin());
  } else {
    // Here I sit opposite declarer
    // Nothing clever in this branch yet
  }

  // Try leading round smallest pip card
  auto suit_cards =
    std::make_pair(plays.begin(), plays.lower_bound(Suit::trumps));

  if (!boost::empty(suit_cards)) {
    auto candidate =
      boost::range::min_element(suit_cards, Card::CompareSuitRanks());

    if (!ai.guaranteed_to_win_against(*candidate, ai.declarer())) {
      return *candidate;
    }
  }

  // Here we're almost certainly screwed.  Play any card that might get the
  // lead somewhere else
  auto candidate = plays.end();
  do {
    --candidate;
    if (!ai.guaranteed_to_win_against(*candidate, other_hands)) {
      return *candidate;
    }
  } while (candidate != plays.begin());

  // Now we really are screwed; all my cards are guaranteed to win.  It
  // doesn't matter what I play
  return *plays.begin();
}

}}

