#include <konig/ai/tai.hpp>

#include <konig/utility/intersects.hpp>

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

  // If there's only one legal play, play it
  if (plays.size() == 1) {
    return *plays.begin();
  }

  std::set<CardFate> hands_yet_to_play;
  {
    PlayPosition pos = ai.position();
    for (size_t i=trick.played(); i<3; ++i) {
      ++pos;
      pos %= 4;
      hands_yet_to_play.insert(CardFate::held_by(pos));
    }
  }

  if (trick.leader() == ai.position()) {
    // I am leading to the trick

    // If I hold the Mond and the Skus might be out there, then lead that
    if (plays.count(TrumpRank::mond) &&
      utility::intersects(hands_yet_to_play, ai.fates_of(TrumpRank::skus))) {
      return Card(TrumpRank::mond);
    }

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

    if (!candidates.empty()) {
      auto best = std::max_element(candidates.begin(), candidates.end());
      if (best->first >= 0) return best->second;
    }
  }

  KONIG_FATAL("not implemented\ntrick " << trick << " hand " << hand);
}

}}

