#include <konig/ai/sddefenceai.hpp>

#include <tuple>

#include <boost/range/empty.hpp>
#include <boost/range/size.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace ai {

void SdDefenceAi::play_start(FateAi const& ai)
{
  guess_master_defender_ = ai.hand().count(Suit::trumps) >= 5;

  suit_profiles_.clear();
  // collect size and value of each suit
  std::map<Suit, std::pair<size_t, size_t>> suits;
  BOOST_FOREACH(const Card& card, ai.hand()) {
    std::map<Suit, std::pair<size_t, size_t>>::iterator i =
      suits.insert(std::make_pair(card.suit(), std::make_pair(0, 0))).first;
    i->second.first += 1;
    i->second.second += card.card_points();
  }
  // Invert the map so we know what our shortest suits are
  typedef std::pair<Suit, std::pair<size_t, size_t>> SuitSizeAndPoints;
  BOOST_FOREACH(const SuitSizeAndPoints& s, suits) {
    suit_profiles_.push_back(
        SuitProfile{s.second.first, s.second.second, s.first}
      );
  }
  std::sort(suit_profiles_.begin(), suit_profiles_.end());
}

std::vector<Announcement> SdDefenceAi::announce(FateAi const&) {
  return std::vector<Announcement>();
}

Card SdDefenceAi::play_card(FateAi const& ai) {
  Trick const& trick = ai.tricks().back();
  //Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();
  boost::optional<Card> bird = ai.relevant_bird();

  // If there's only one legal play, play it
  if (plays.size() == 1) {
    return *plays.begin();
  }

  if (trick.played() == 0) {
    // We're leading
    if (guess_master_defender_) {
      // Lead a long suit
      for (SuitProfiles::reverse_iterator it = suit_profiles_.rbegin();
        it != suit_profiles_.rend(); ++it) {
        if (it->suit == Suit::trumps) continue;
        auto playable = plays.equal_range(it->suit);
        if (!boost::empty(playable)) {
          return *playable.first;
        }
      }
    } else {
      // Lead a short suit
      for (SuitProfiles::iterator it = suit_profiles_.begin();
        it != suit_profiles_.end(); ++it) {
        if (it->suit == Suit::trumps) continue;
        auto playable = plays.equal_range(it->suit);
        if (!boost::empty(playable)) {
          return *playable.first;
        }
      }
    }
    // At this point we must be playing trumps; play low
    assert(plays.begin()->trump());
    auto candidate = plays.begin();
    // Avoid leading a bird that might cause us to lose game points
    if (bird && *candidate == *bird) ++candidate;
    return *candidate;
  } else {
    // We're following
    Suit s = trick.suit();
    bool const playing_trump = plays.begin()->trump();
    bool const roughing = playing_trump && s != Suit::trump;

    const int declarers_position_in_trick =
      (ai.declarer() - trick.leader() + 4)%4;
    const bool declarer_has_played =
      trick.played() > declarers_position_in_trick;

    if (declarer_has_played) {
      bool const declarer_is_winning =
        trick.winner() == ai.declarer();

      if (declarer_is_winning) {
        auto winning_play = plays.lower_bound(trick.winning_card());
        if (!winning_play->trump() && winning_play->suit() != s) {
          winning_play = plays.end();
        }

        if (roughing) {
          // If I'm last to play then make an effort to win
          if (trick.played() == 3 && winning_play != plays.end()) {
            return *winning_play;
          }
          // Otherwise just play low
          return *plays.begin();
        } else if (playing_trump) {
          // Following in trumps
          if (winning_play != plays.end()) {
            // Win minimally
            return *winning_play;
          } else {
            // Can't win; play low
            return *plays.begin();
          }
        } else {
          // Declarer is winning and I'm following suit or discarding
          if (winning_play != plays.end()) {
            // We can win in a side suit; play card worth most points, or least
            // valuable winning pip card
            auto most_points = std::max_element(
              winning_play, plays.end(), Card::CompareRanksReversePips()
            );
            return *most_points;
          }

          // Can't win, so play low
          /** \bug Discarding something valuable sometimes under these
           * circumstances is pretty vital */
          auto smallest_rank = std::min_element(
            plays.begin(), plays.end(), Card::CompareRanks()
          );
          return *smallest_rank;
        }
      } else {
        if (roughing) {
          // Play smallest trump
          return *plays.begin();
        } else if (playing_trump) {
          // I'm following in trumps, but the defense is already going to win,
          // so just play low
          /** \bug Really person before declarer should deliberately beat
           * partener sometimes */
          return *plays.begin();
        } else {
          // Play card worth most points, or least valuable pip card
          auto most_points = std::max_element(
            plays.begin(), plays.end(), Card::CompareRanksReversePips()
          );
          return *most_points;
        }
      }
    } else {
      // Declarer hasn't played yet.
      bool const defence_will_win =
        ai.guaranteed_to_win_against(trick.winning_card(), ai.declarer());
      if (defence_will_win) {
        if (playing_trump) {
          // We're winning anyway, so play a low trump
          return *plays.begin();
        } else {
          // Play something worth points
          auto most_points = std::max_element(
            plays.begin(), plays.end(), Card::CompareRanksReversePips()
          );
          return *most_points;
        }
      } else {
        // Defence not guaranteed to win, so declarer probably will.  Play
        // something worthless
        auto smallest_rank = std::min_element(
          plays.begin(), plays.end(), Card::CompareRanks()
        );
        return *smallest_rank;
      }
    }
  }

  KONIG_FATAL("unimplimented (" << ai.tricks().size() << "): " << trick);
}

}}

