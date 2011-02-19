#include <konig/ai/get_dangerous_suit_candidates.hpp>

namespace konig { namespace ai {

std::vector<std::pair<int, Card>>
  get_dangerous_suit_candidates(FateAi const& ai)
{
  Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();

  typedef int Score;
  std::vector<std::pair<Score, Card>> candidates;

  // Look for a good King to underlead or Queen to lead
  for (Suit suit = Suit::min; suit < Suit::trumps; ++suit) {
    if (ai.had_first_round(suit)) continue;

    Card king(suit, SuitRank::king);
    Card queen(suit, SuitRank::queen);
    size_t size = plays.count(suit);
    auto king_i = plays.find(king);
    if (king_i != plays.end() && size > 1) {
      Card candidate = *plays.lower_bound(suit);
      bool gap = false;
      for (SuitRank r = SuitRank::queen; r > candidate.suit_rank(); --r) {
        if (!hand.count(Card(suit, r))) {
          gap = true;
        } else if (gap) {
          candidate = Card(suit, r);
        }
      }
      // HACK: arbitrary constant 3 multiplying size
      Score score = king.card_points() - 3*size;
      if (!gap) score -= candidate.card_points();
      candidates.push_back(std::make_pair(score, candidate));
    }
    if (plays.count(queen)) {
      // HACK: arbitrary constant 3 multiplying size
      Score score = queen.card_points() - 3*size;
      Card knight(suit, SuitRank::knight);
      if (hand.count(knight)) score -= knight.card_points();
      candidates.push_back(std::make_pair(score, queen));
    }
  }
  return candidates;
}

}}

