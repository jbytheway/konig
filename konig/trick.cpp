#include <konig/trick.hpp>

#include <boost/range/empty.hpp>
#include <boost/format.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>

#include <konig/contractandannouncements.hpp>

namespace konig {

void Trick::add(Card card) {
  assert(!complete());
  cards_[played_++] = card;
  if (played_ == 1) {
    winning_card_ = 0;
  } else {
    Suit this_suit = card.suit();
    if (this_suit == Suit::trumps || this_suit == suit()) {
      if (cards_[winning_card_] < card) {
        winning_card_ = played_-1;
      }
    }
  }
}

Cards Trick::legal_plays(
    const Cards& hand,
    bool offence,
    unsigned int trick,
    const ContractAndAnnouncements& whole_contract
  ) const
{
  Cards legal_plays;
  bool following_suit_or_roughing = false;
  if (played_ == 0) {
    // Leading card to the trick
    legal_plays = hand;
  } else {
    // Must follow suit or else trump or else anything
    following_suit_or_roughing = true;
    Suit trick_suit = cards_[0].suit();
    legal_plays = hand.subset(trick_suit);
    if (boost::empty(legal_plays)) {
      legal_plays = hand.subset(Suit::trump);
      if (boost::empty(legal_plays)) {
        legal_plays = hand;
        following_suit_or_roughing = false;
      }
    }
  }
  // Apply rising rule
  if (following_suit_or_roughing && rising_rule_) {
    auto first_winner = legal_plays.upper_bound(winning_card());
    if (first_winner != legal_plays.end()) {
      legal_plays = legal_plays.subset(first_winner, legal_plays.end());
    }
  }
  // Apply hold-pagat rule
  Card pagat(TrumpRank::pagat);
  if (hold_pagat_ && legal_plays.count(pagat) &&
      legal_plays.count(Suit::trumps) != 1) {
    legal_plays.erase(pagat);
  }
  std::array<Cards, 3> constraint_levels;
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
  return real_plays;
}

std::ostream& operator<<(std::ostream& o, const Trick& t)
{
  if (!t.played()) {
    o << "-- -- -- --";
  } else {
    for (PlayPosition p = position_forehand; p < position_max; ++p) {
      int index = (p-t.leader()+4)%4;
      if (index < t.played()) {
        Card c = t.cards()[index];
        o << boost::format("%2s") % c;
      } else {
        o << "--";
      }
      if (p == t.winner()) {
        o << '*';
      } else {
        o << ' ';
      }
    }
  }
  return o;
}

}

