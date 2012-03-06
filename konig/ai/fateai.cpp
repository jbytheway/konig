#include <konig/ai/fateai.hpp>

#include <boost/range/algorithm/fill.hpp>

#include <konig/fatal.hpp>
#include <konig/utility/intersects.hpp>
#include <konig/utility/intersection_into.hpp>
#include <konig/utility/union_into.hpp>

namespace konig { namespace ai {

FateAi::FateAi() :
  fates_(Card::index_max)
{
}

void FateAi::start_game(Ruleset rules, PlayPosition pos, Cards hand)
{
  Ai::start_game(std::move(rules), pos, std::move(hand));

  boost::fill(had_first_round_, false);
  boost::fill(num_tricks_taken_by_, 0);
  called_suit_ = Suit::trumps;
  CardFates my_hand{CardFate::held_by(pos)};
  CardFates other_places{
    CardFate::hand0, CardFate::hand1, CardFate::hand2, CardFate::hand3,
    CardFate::talon
  };
  other_places.erase(*my_hand.begin());
  assert(other_places.size() == 4);

  for (Card::index_type c = 0; c != Card::index_max; ++c) {
    if (this->hand().count(Card::from_index(c))) {
      fates_[c] = my_hand;
    } else {
      fates_[c] = other_places;
    }
  }
}

void FateAi::notify_call_king(KingCall call)
{
  Ai::notify_call_king(call);

  if (call == KingCall::fourth_king) {
    KONIG_FATAL("not implemented");
  } else {
    called_suit_ = Suit::from_value(call);
  }
}

void FateAi::notify_talon(const std::array<Cards, 2>& talon)
{
  Ai::notify_talon(talon);

  CardFates declarers_hand_or_discarded{
    CardFate::held_by(declarer()), CardFate::discard
  };
  BOOST_FOREACH(Card const& c, accepted()) {
    fates_[c.index()] = declarers_hand_or_discarded;
  }

  for (Card::index_type i = 0; i != Card::index_max; ++i) {
    auto& fates = fates_[i];
    auto const c = Card::from_index(i);
    if (fates.count(CardFate::talon)) {
      fates.erase(CardFate::talon);
      if (!c.trump() && !c.suit_rank() == SuitRank::king) {
        fates.insert(CardFate::discard);
      }
    }
  }
}

void FateAi::notify_talon_choice(uint8_t choice)
{
  Ai::notify_talon_choice(choice);

  CardFates rejected{CardFate::reject};
  BOOST_FOREACH(Card const& c, this->rejected()) {
    fates_[c.index()] = rejected;
  }
}

void FateAi::notify_discard(Cards discard)
{
  Ai::notify_discard(std::move(discard));

  CardFates discarded{CardFate::discard};
  BOOST_FOREACH(Card const& c, discard) {
    fates_[c.index()] = discarded;
  }
}

void FateAi::notify_play_card(PlayPosition p, Card c)
{
  CardFates played_by_p{CardFate::played_by(p)};
  fates_[c.index()] = played_by_p;
  bool const rising_rule = contract().contract().rising_rule();

  auto const players_hand = CardFate::held_by(p);
  Trick const& trick = tricks().back();
  if (trick.played()) {
    if (trick.suit() == Suit::trumps) {
      if (!c.trump()) {
        BOOST_FOREACH(auto& fates, fates_of(Suit::trumps)) {
          fates.erase(players_hand);
        }
      } else if (rising_rule && c < trick.winning_card()) {
        auto winners_start = trick.winning_card().index();
        for (auto i = winners_start; i != Card::index_max; ++i) {
          fates_[i].erase(players_hand);
        }
      }
    } else {
      if (c.suit() != trick.suit()) {
        BOOST_FOREACH(auto& fates, fates_of(trick.suit())) {
          fates.erase(players_hand);
        }
        if (!c.trump()) {
          BOOST_FOREACH(auto& fates, fates_of(Suit::trumps)) {
            fates.erase(players_hand);
          }
        }
      } else if (rising_rule && !trick.winning_card().trump() &&
          c < trick.winning_card()) {
        auto winners_start = trick.winning_card().index();
        auto suit_end = Card(c.suit(), SuitRank::king).index();
        for (auto i = winners_start; i <= suit_end; ++i) {
          fates_[i].erase(players_hand);
        }
      }
    }
  }

  if (c == Card(TrumpRank::pagat) && contract().contract().hold_pagat()) {
    BOOST_FOREACH(auto& fates, fates_of(Suit::trumps)) {
      fates.erase(players_hand);
    }
  }

  Ai::notify_play_card(p, c);
}

void FateAi::trick_complete_hook()
{
  auto const& trick = tricks().back();
  had_first_round_[trick.suit()] = true;
  num_tricks_taken_by_[trick.winner()]++;
}

Suit FateAi::called_suit(int const fallback) const
{
  if (called_suit_ == Suit::trumps) {
    if (fallback == Suit::max) {
      KONIG_FATAL("don't know called suit yet");
    }
    return Suit(fallback);
  }

  return called_suit_;
}

bool FateAi::is_called_suit(Suit const s) const
{
  if (!contract().contract().is_partnership()) {
    return false;
  }
  return s == called_suit();
}

bool FateAi::guess_is_partner(PlayPosition const pos) const
{
  if (!contract().contract().is_partnership()) {
    return false;
  }
  Card called_king(called_suit(), SuitRank::king);
  auto const& fates = fates_[called_king.index()];
  if (fates.size() == 1) {
    auto fate = *fates.begin();
    return fate == CardFate::held_by(pos) || fate == CardFate::played_by(pos);
  }
  // TODO: see who's led trumps
  return false;
}

bool FateAi::guess_is_on_my_side(PlayPosition const pos) const
{
  if (position() == pos) return true;

  if (position() == declarer()) {
    return guess_is_partner(pos);
  } else if (offence()) {
    return declarer() == pos;
  } else {
    return !guess_is_partner(pos);
  }
}

int FateAi::seats_after_declarer() const
{
  return (position()-declarer()+4)%4;
}

bool FateAi::had_first_round(Suit const s) const
{
  assert(s < Suit::max);
  assert(s >= 0);
  return had_first_round_[s];
}

FateAi::CardFates FateAi::fates_of(Card const& card) const
{
  return fates_[card.index()];
}

FateAi::CardFates FateAi::fates_of(TrumpRank const rank) const
{
  return fates_of(Card(rank));
}

FateAi::CardFates FateAi::other_players_hands() const
{
  CardFates hands{
    CardFate::hand0, CardFate::hand1, CardFate::hand2, CardFate::hand3
  };
  hands.erase(CardFate::held_by(position()));
  return hands;
}

std::pair<FateAi::CardsFates::iterator, FateAi::CardsFates::iterator>
FateAi::fates_of(Suit const s)
{
  if (s == Suit::trumps) {
    return {fates_.begin() + Card::index_first_trump, fates_.end()};
  } else {
    return {
      fates_.begin() + Card(s).index(),
      fates_.begin() + Card(s, SuitRank::king).index() + 1
    };
  }
}

std::pair<
  FateAi::CardsFates::const_iterator,
  FateAi::CardsFates::const_iterator
>
FateAi::fates_of(Suit const s) const
{
  if (s == Suit::trumps) {
    return {fates_.begin() + Card(TrumpRank::pagat).index(), fates_.end()};
  } else {
    return {
      fates_.begin() + Card(s).index(),
      fates_.begin() + Card(s, SuitRank::king).index() + 1
    };
  }
}

Cards FateAi::trumps_in(CardFates const& places) const
{
  Cards result;
  for (auto i = Card(TrumpRank::min).index(); i != Card::index_max; ++i) {
    CardFates const& fates = fates_[i];
    using utility::intersects;
    if (intersects(places, fates)) result.insert(Card::from_index(i));
  }
  return result;
}

Cards FateAi::trumps_out() const
{
  auto hands = other_players_hands();
  return trumps_in(hands);
}

bool FateAi::trumps_known_exhausted() const
{
  return trumps_out().empty();
}

size_t FateAi::num_players_known_out_of_trumps() const
{
  CardFates result;
  BOOST_FOREACH(auto const& fates, fates_of(Suit::trumps)) {
    using utility::union_into;
    union_into(result, fates);
  }
  CardFates hands{
    CardFate::hand0, CardFate::hand1, CardFate::hand2, CardFate::hand3
  };
  using utility::intersection_into;
  intersection_into(result, hands);
  return 4-result.size();
}

bool FateAi::guaranteed_to_win_against(
  Card const& card,
  CardFates const& hands
) const
{
  if (hands.empty()) return true;

  /** \bug Could be made faster */
  if (card.trump()) {
    for (TrumpRank better_rank = boost::next(card.trump_rank());
      better_rank < TrumpRank::max; ++better_rank) {
      using utility::intersects;
      if (intersects(fates_[Card(better_rank).index()], hands)) {
        return false;
      }
    }
    return true;
  } else {
    for (SuitRank better_rank = boost::next(card.suit_rank());
      better_rank < SuitRank::max; ++better_rank) {
      auto const& fates = fates_[Card(card.suit(), better_rank).index()];
      using utility::intersects;
      if (intersects(fates, hands)) {
        return false;
      }
    }
    BOOST_FOREACH(auto const& fates, fates_of(Suit::trumps)) {
      using utility::intersects;
      if (intersects(fates, hands)) {
        return false;
      }
    }
    return true;
  }
}

bool FateAi::guaranteed_to_win_against(
  Card const& card,
  PlayPosition const pos
) const
{
  CardFates in_pos_hand{CardFate::held_by(pos)};
  return guaranteed_to_win_against(card, in_pos_hand);
}

size_t FateAi::num_tricks_taken_by(PlayPosition const pos) const
{
  return num_tricks_taken_by_[pos];
}

bool FateAi::cards_are_equivalent(Cards const& cards) const
{
  if (cards.size() <= 1) return true;

  Suit const suit = cards.begin()->suit();
  auto const last_it = boost::prior(cards.end());

  if (last_it->suit() != suit) {
    return false;
  }

  auto fates_range = std::make_pair(
    fates_.begin() + cards.begin()->index() + 1,
    fates_.begin() + last_it->index()
  );

  auto other_hands = other_players_hands();

  BOOST_FOREACH(auto const& fates, fates_range) {
    using utility::intersects;
    if (intersects(fates, other_hands)) {
      return false;
    }
  }

  return true;
}

}}

