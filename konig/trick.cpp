#include <konig/trick.hpp>

#include <boost/range/empty.hpp>
#include <boost/format.hpp>

#include <konig/contractandannouncements.hpp>

namespace konig {

Cards Trick::legal_plays(
    const Cards& hand,
    bool offence,
    unsigned int trick,
    const ContractAndAnnouncements& whole_contract
  ) const
{
  std::pair<Cards::iterator, Cards::iterator> legal_plays;
  if (played_ == 0) {
    // Leading card to the trick
    legal_plays = make_pair(hand.begin(), hand.end());
  } else {
    // Must follow suit or else trump or else anything
    Suit trick_suit = cards_[0].suit();
    legal_plays = hand.equal_range(trick_suit);
    if (boost::empty(legal_plays)) {
      legal_plays = hand.equal_range(Suit::trump);
      if (boost::empty(legal_plays)) {
        legal_plays = make_pair(hand.begin(), hand.end());
      }
    }
  }
  boost::array<Cards, 3> constraint_levels;
  BOOST_FOREACH(Card legal_play, legal_plays) {
    constraint_levels[
        whole_contract.play_constraint(legal_play, offence, trick)
      ].insert(legal_play);
  }
  Cards real_plays;
  if (!constraint_levels[PlayConstraint::try_to_play].empty()) {
    real_plays = std::move(constraint_levels[PlayConstraint::try_to_play]);
  } else if (!constraint_levels[PlayConstraint::neutral].empty()) {
    real_plays = std::move(constraint_levels[PlayConstraint::neutral]);
  } else {
    real_plays = std::move(constraint_levels[PlayConstraint::avoid_playing]);
  }
  assert(!real_plays.empty());
  return std::move(real_plays);
}

void Trick::determine_winner()
{
  assert(played_ == 4);
  Suit trick_suit = cards_[0].suit();
  uint8_t best_card = 0;
  for (uint8_t i=1; i<4; ++i) {
    Suit suit = cards_[i].suit();
    if (suit == Suit::trumps || suit == trick_suit) {
      if (cards_[best_card] < cards_[i]) {
        best_card = i;
      }
    }
  }
  winner_ = PlayPosition((leader_ + best_card) % 4);
}

std::ostream& operator<<(std::ostream& o, const Trick& t)
{
  assert(t.complete());
  for (PlayPosition p = position_forehand; p < position_max; ++p) {
    Card c = t.cards()[(p-t.leader()+4)%4];
    o << boost::format("%2s") % c;
    if (p == t.winner()) {
      o << '*';
    } else {
      o << ' ';
    }
  }
  return o;
}

}

