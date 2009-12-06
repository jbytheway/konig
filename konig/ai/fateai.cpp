#include <konig/ai/fateai.hpp>

#include <boost/assign/list_of.hpp>

#include <konig/utility/intersects.hpp>

namespace konig { namespace ai {

void FateAi::start_game(Ruleset rules, PlayPosition pos, Cards hand)
{
  Ai::start_game(std::move(rules), pos, std::move(hand));

  fates_.clear();
  std::vector<Card> deck;
  Card::make_deck(std::back_inserter(deck));
  std::set<CardFate> my_hand = boost::assign::list_of(CardFate(pos));
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

void FateAi::notify_talon(const std::array<Cards, 2>& talon)
{
  Ai::notify_talon(talon);

  std::set<CardFate> declarers_hand_or_discarded =
    boost::assign::list_of(CardFate(declarer()))(CardFate::discard);
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
  BOOST_FOREACH(Card const& c, this->discard()) {
    fates_[c] = discarded;
  }
}

void FateAi::notify_play_card(PlayPosition p, Card c)
{
  std::set<CardFate> played =
    boost::assign::list_of(CardFate::played);
  fates_[c] = played;

  CardFate players_hand(p);
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

Cards FateAi::trumps_out() const
{
  Cards result;
  std::set<CardFate> hands =
      boost::assign::list_of
        (CardFate::hand0)(CardFate::hand1)(CardFate::hand2)(CardFate::hand3);
  hands.erase(CardFate(position()));
  BOOST_FOREACH(auto const& p, fates_of(Suit::trumps)) {
    std::set<CardFate> const& fates = p.second;
    if (utility::intersects(hands, fates)) result.insert(p.first);
  }
  return result;
}

bool FateAi::trumps_known_exhausted() const
{
  return trumps_out().empty();
}

}}

