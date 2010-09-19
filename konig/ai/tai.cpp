#include <konig/ai/tai.hpp>

#include <boost/assign/list_of.hpp>

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

  std::set<CardFate> other_hands = boost::assign::list_of
    (CardFate::hand0)(CardFate::hand1)(CardFate::hand2)(CardFate::hand3);
  other_hands.erase(CardFate::held_by(ai.position()));

  bool const skus_may_be_out =
    utility::intersects(other_hands, ai.fates_of(TrumpRank::skus));

  if (trick.leader() == ai.position()) {
    // I am leading to the trick

    // If I hold the Mond and the Skus might be out there, then lead that
    if (plays.count(TrumpRank::mond) && skus_may_be_out) {
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
  } else {
    // I am following to the trick
    Suit s = trick.suit();
    bool const will_rise =
      (plays.begin()->trump() && s != Suit::trumps) ||
      (plays.begin()->suit() == s && *plays.begin() > trick.winning_card());

    if (!will_rise) {
      // I'm not going to win, so I should discard the most dangerous card I
      // can
      std::vector<Card> p(plays.begin(), plays.end());
      return *std::max_element(p.begin(), p.end(), Card::CompareRanks());
    }

    if (s == Suit::trumps) {
      // Trump trick
    } else {
      // Suit trick

      if (plays.begin()->suit() == Suit::trumps) {
        // I am roughing

        if (hands_yet_to_play.empty()) {
          // I'm last to play, so I am certain to win.  Play biggest trump I
          // have
          auto candidate = boost::prior(plays.end());
          // ... except not the Mond if the Skus is still out
          if (candidate != plays.begin() &&
            candidate->trump_rank() == TrumpRank::mond && skus_may_be_out) {
            --candidate;
          }
          return *candidate;
        }
      } else {
        // I am following to a suit trick
        if (hands_yet_to_play.empty()) {
          // I'm last to play, so I am certain to win.  Do so in the manner to
          // gain me fewest points
          return *std::min_element(
            plays.begin(), plays.end(), Card::CompareRanksReversePips()
          );
        }

        if (!ai.had_first_round(s)) {
          // First round of this; probably an underled King; just about
          // anything from Jack upwards will win, so again try to gain fewest
          // points
          return *std::min_element(
            plays.begin(), plays.end(), Card::CompareRanksReversePips()
          );
        }
      }
    }
  }

  KONIG_FATAL("not implemented\ntrick " << trick << " hand " << hand);
}

}}

