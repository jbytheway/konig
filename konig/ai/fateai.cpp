#include <konig/ai/fateai.hpp>

#include <boost/assign/list_of.hpp>

#include <konig/fatal.hpp>
#include <konig/utility/intersects.hpp>
#include <konig/utility/intersection_into.hpp>
#include <konig/utility/union_into.hpp>

namespace konig { namespace ai {

FateAi::FateAi() :
  called_suit_(Suit::trumps)
{
  std::fill(had_first_round_.begin(), had_first_round_.end(), false);
}

void FateAi::start_game(Ruleset rules, PlayPosition pos, Cards hand)
{
  Ai::start_game(std::move(rules), pos, std::move(hand));

  fates_.clear();
  std::vector<Card> deck;
  Card::make_deck(std::back_inserter(deck));
  std::set<CardFate> my_hand = boost::assign::list_of(CardFate::held_by(pos));
  std::set<CardFate> other_places =
    boost::assign::list_of
      (CardFate::hand0)(CardFate::hand1)(CardFate::hand2)(CardFate::hand3)
      (CardFate::talon);
  other_places.erase(*my_hand.begin());
  assert(other_places.size() == 4);

  BOOST_FOREACH(Card const& c, deck) {
    if (this->hand().count(c)) {
      fates_.insert(std::make_pair(c, my_hand));
    } else {
      fates_.insert(std::make_pair(c, other_places));
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

  std::set<CardFate> declarers_hand_or_discarded =
    boost::assign::list_of(CardFate::held_by(declarer()))(CardFate::discard);
  BOOST_FOREACH(Card const& c, accepted()) {
    fates_[c] = declarers_hand_or_discarded;
  }

  BOOST_FOREACH(auto& p, fates_) {
    if (p.second.count(CardFate::talon)) {
      p.second.erase(CardFate::talon);
      if (!p.first.trump() && !p.first.suit_rank() == SuitRank::king) {
        p.second.insert(CardFate::discard);
      }
    }
  }
}

void FateAi::notify_talon_choice(uint8_t choice)
{
  Ai::notify_talon_choice(choice);

  std::set<CardFate> rejected =
    boost::assign::list_of(CardFate::reject);
  BOOST_FOREACH(Card const& c, this->rejected()) {
    fates_[c] = rejected;
  }
}

void FateAi::notify_discard(Cards discard)
{
  Ai::notify_discard(std::move(discard));

  std::set<CardFate> discarded =
    boost::assign::list_of(CardFate::discard);
  BOOST_FOREACH(Card const& c, discard) {
    fates_[c] = discarded;
  }
}

void FateAi::notify_play_card(PlayPosition p, Card c)
{
  std::set<CardFate> played_by_p =
    boost::assign::list_of(CardFate::played_by(p));
  fates_[c] = played_by_p;

  CardFate players_hand = CardFate::held_by(p);
  Trick const& trick = tricks().back();
  if (trick.played()) {
    if (trick.suit() == Suit::trumps) {
      if (!c.trump()) {
        BOOST_FOREACH(auto& p, fates_of(Suit::trumps)) {
          p.second.erase(players_hand);
        }
      }
    } else {
      if (c.suit() != trick.suit()) {
        BOOST_FOREACH(auto& p, fates_of(trick.suit())) {
          p.second.erase(players_hand);
        }
        if (c.suit() != Suit::trumps) {
          BOOST_FOREACH(auto& p, fates_of(Suit::trumps)) {
            p.second.erase(players_hand);
          }
        }
      }
    }
  }

  Ai::notify_play_card(p, c);
}

void FateAi::trick_complete_hook()
{
  had_first_round_[tricks().back().suit()] = true;
}

Suit FateAi::called_suit() const
{
  if (called_suit_ == Suit::trumps) {
    KONIG_FATAL("don't know called suit yet");
  }

  return called_suit_;
}

bool FateAi::is_called_suit(Suit const s) const
{
  if (!contract().contract()->is_partnership()) {
    return false;
  }
  return s == called_suit();
}

bool FateAi::guess_is_partner(PlayPosition const pos) const
{
  if (!contract().contract()->is_partnership()) {
    return false;
  }
  Card called_king(called_suit(), SuitRank::king);
  auto const& fates = fates_.find(called_king)->second;
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

bool FateAi::had_first_round(Suit const s) const
{
  assert(s < Suit::max);
  assert(s >= 0);
  return had_first_round_[s];
}

std::set<CardFate> FateAi::fates_of(Card const& card) const
{
  return fates_.find(card)->second;
}

std::set<CardFate> FateAi::fates_of(TrumpRank const rank) const
{
  return fates_of(Card(rank));
}

std::pair<FateAi::Fates::iterator, FateAi::Fates::iterator>
FateAi::fates_of(Suit const s)
{
  if (s == Suit::trumps) {
    return
      std::make_pair(fates_.lower_bound(Card(TrumpRank::pagat)), fates_.end());
  } else {
    return std::make_pair(
        fates_.lower_bound(Card(s, SuitRank::low_pip)),
        fates_.upper_bound(Card(s, SuitRank::king))
      );
  }
}

std::pair<FateAi::Fates::const_iterator, FateAi::Fates::const_iterator>
FateAi::fates_of(Suit const s) const
{
  if (s == Suit::trumps) {
    return
      std::make_pair(fates_.lower_bound(Card(TrumpRank::pagat)), fates_.end());
  } else {
    return std::make_pair(
        fates_.lower_bound(Card(s, SuitRank::low_pip)),
        fates_.upper_bound(Card(s, SuitRank::king))
      );
  }
}

Cards FateAi::trumps_in(std::set<CardFate> const& places) const
{
  Cards result;
  BOOST_FOREACH(auto const& p, fates_of(Suit::trumps)) {
    std::set<CardFate> const& fates = p.second;
    if (utility::intersects(places, fates)) result.insert(p.first);
  }
  return result;
}

Cards FateAi::trumps_out() const
{
  std::set<CardFate> hands =
      boost::assign::list_of
        (CardFate::hand0)(CardFate::hand1)(CardFate::hand2)(CardFate::hand3);
  hands.erase(CardFate::held_by(position()));
  return trumps_in(hands);
}

bool FateAi::trumps_known_exhausted() const
{
  return trumps_out().empty();
}

size_t FateAi::num_players_known_out_of_trumps() const
{
  std::set<CardFate> result;
  BOOST_FOREACH(auto const& p, fates_of(Suit::trumps)) {
    std::set<CardFate> const& fates = p.second;
    utility::union_into(result, fates);
  }
  std::set<CardFate> hands =
      boost::assign::list_of
        (CardFate::hand0)(CardFate::hand1)(CardFate::hand2)(CardFate::hand3);
  utility::intersection_into(result, hands);
  return 4-result.size();
}

bool FateAi::guaranteed_to_win_against(
  Card const& card,
  std::set<CardFate> const& hands
) const
{
  if (hands.empty()) return true;

  /** \bug Could be made faster */
  if (card.trump()) {
    for (TrumpRank better_rank = boost::next(card.trump_rank());
      better_rank < TrumpRank::max; ++better_rank) {
      if (utility::intersects(fates_.find(Card(better_rank))->second, hands)) {
        return false;
      }
    }
    return true;
  } else {
    for (SuitRank better_rank = boost::next(card.suit_rank());
      better_rank < SuitRank::max; ++better_rank) {
      auto p = fates_.find(Card(card.suit(), better_rank));
      assert(p != fates_.end());
      if (utility::intersects(p->second, hands)) {
        return false;
      }
    }
    BOOST_FOREACH(auto const& p, fates_of(Suit::trumps)) {
      if (utility::intersects(p.second, hands)) {
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
  std::set<CardFate> in_pos_hand =
    boost::assign::list_of(CardFate::held_by(pos));
  return guaranteed_to_win_against(card, in_pos_hand);
}

}}

