#include <konig/ai/sdoffenceai.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace ai {

OffenceAi::OffenceAi() :
  king_rippiness_penalty_{10},
  trumps_for_unannounced_bird_{5, 6, 7},
  trumps_out_to_abort_pagat_{2},
  trumps_out_to_abort_bird_{4, 6, 10},
  min_desired_rippiness_{3},
  valuable_card_points_{10}
{
  assert(trumps_for_unannounced_bird_.size() == 3);
  assert(std::adjacent_find(
      trumps_for_unannounced_bird_.begin(), trumps_for_unannounced_bird_.end(),
      arg1 > arg2
    ) == trumps_for_unannounced_bird_.end());
  assert(std::adjacent_find(
      trumps_out_to_abort_bird_.begin(), trumps_out_to_abort_bird_.end(),
      arg1 > arg2
    ) == trumps_out_to_abort_bird_.end());
}

void OffenceAi::reset(FateAi const& ai)
{
  num_voids_ = 0;
  for (Suit s = Suit::min; s<Suit::trumps; ++s) {
    Cards in_suit(ai.hand().equal_range(s));
    num_voids_ += in_suit.empty();
    void_waiting_[s] = in_suit.empty();
    rippiness_[s] = in_suit.size();
    if (in_suit.count(SuitRank::king)) {
      rippiness_[s] -= king_rippiness_penalty_;
    }
  }
  // Determine which birds are even vaguely worth trying for
  TrumpRank first_non_bird(4);
  size_t num_trumps = ai.hand().count(Suit::trumps);
  BOOST_FOREACH(auto const limit, trumps_for_unannounced_bird_) {
    if (num_trumps < limit) --first_non_bird;
  }
  assert(first_non_bird >= TrumpRank::pagat);
  // Get non-bird trumps
  Cards non_bird_trumps(
    ai.hand().lower_bound(Card(first_non_bird)), ai.hand().end()
  );
  if (non_bird_trumps.empty()) {
    lowest_trump_to_rough_ = Card(TrumpRank::skus);
  } else {
    lowest_trump_to_rough_ = *non_bird_trumps.begin();
  }
  if (non_bird_trumps.size() <= num_voids_) {
    lowest_trump_to_lead_ = Card(TrumpRank::skus);
  } else {
    lowest_trump_to_lead_ = *boost::next(non_bird_trumps.begin(), num_voids_);
  }
}

std::vector<Announcement> OffenceAi::announce(FateAi const&)
{
  return std::vector<Announcement>();
}

Card OffenceAi::play_card(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();
  size_t const trick_number = ai.tricks().size();
  size_t const num_trumps_out = ai.trumps_out().size();
  std::set<CardFate> hands_yet_to_play;
  {
    PlayPosition pos = ai.position();
    for (size_t i=trick.played(); i<3; ++i) {
      ++pos;
      pos %= 4;
      hands_yet_to_play.insert(CardFate(pos));
    }
  }
  size_t const trumps_in_hands_yet_to_play =
    ai.trumps_in(hands_yet_to_play).size();

  // If there's only one legal play, play it
  if (plays.size() == 1) {
    return *plays.begin();
  }

  boost::optional<Card> bird = ai.relevant_bird();

  // Cash a bird if it's safe to
  if (bird && plays.count(*bird) && trumps_in_hands_yet_to_play == 0) {
    // Also need to make sure that there isn't a better card in the trick
    // already
    if (trick.played() == 0 || trick.winning_card() < *bird) {
      return *bird;
    }
  }

  // Abort Pagat on Uhu trick if there are at least 2 trumps unaccounted for
  if (plays.count(Card(TrumpRank::pagat)) && trick_number == 11 &&
        num_trumps_out >= trumps_out_to_abort_pagat_) {
    return Card(TrumpRank::pagat);
  }

  if (trick.leader() == ai.position()) {
    // I am leading to the trick

    // More complex abortion logic
    {
      TrumpRank abortable_bird = TrumpRank::kakadu;
      if (trick_number == 10) {
        abortable_bird = TrumpRank::uhu;
      } else if (trick_number == 11) {
        abortable_bird = TrumpRank::pagat;
      }
      auto const before_birds = plays.lower_bound(Suit::trumps);
      auto const after_birds = plays.upper_bound(Card(abortable_bird));
      if (before_birds != after_birds &&
        after_birds == boost::next(before_birds) &&
        after_birds == plays.end())
      {
        // We are looking at a bird and that's the only trump we have (except
        // perhaps for announced birds)
        Card const bird = *before_birds;
        size_t const tricks_of_grace = 12 - bird.trump_rank() - trick_number;
        // Zero means that this is our last chance to abort, larger numbers
        // more tricks of grace.  Remember that in some contracts
        // num_trumps_out includes those in the talon (probably 1 or 2)
        assert(tricks_of_grace < 11);
        if (tricks_of_grace < trumps_out_to_abort_bird_.size() &&
            num_trumps_out >= trumps_out_to_abort_bird_[tricks_of_grace]) {
          return bird;
        }
      }
    }

    // First we check through the suits and make sure that we don't try to rip
    // in one we have no cards in
    for (Suit s = Suit::min; s<Suit::trump; ++s) {
      // But not if this is the last card in the suit
      if (hand.count(s) == 0) {
        rippiness_[s] = -10;
      }
    }
    // Try ripping in a side suit.  Do this if either it's a good suit
    // for ripping or we have at most as many trumps as non-trumps
    auto suit_it = std::max_element(rippiness_.begin(), rippiness_.end());
    Suit suit = Suit::from_value(suit_it-rippiness_.begin());
    if (*suit_it >= min_desired_rippiness_ ||
        hand.count(Suit::trumps)*2 <= hand.size()) {
      // Make more rippy so we're inclined to continue on the same suit
      ++*suit_it;
      assert(plays.count(suit));
      return *plays.lower_bound(suit);
    }
    // No worthwhile ripping suits, so lead trumps
    auto potential_trump = plays.lower_bound(lowest_trump_to_lead_);
    if (potential_trump == plays.end()) {
      // We've exhausted all trumps expect plausible birds and those saved for
      // roughing.  Look for one of the latter
      potential_trump = plays.lower_bound(lowest_trump_to_rough_);
    }
    while (potential_trump != plays.end()) {
      // Make sure we don't play this trick's bird
      if (bird && *potential_trump == *bird && !ai.trumps_known_exhausted()) {
        ++potential_trump;
        continue;
      }
      // It's dangerous to lead the uhu on the kakadu trick if we hold the
      // pagat and a non-trump, because we might have to follow suit on the
      // uhu trick and go off in pagat.  So, try to avoid that.
      // TODO: Even better if we check whether we could possibly be forced to
      // follow suit
      if (hand.size() == 3 &&
          potential_trump->trump_rank() == TrumpRank::uhu &&
          hand.count(TrumpRank::pagat) &&
          hand.count(Suit::trumps) == 2) {
        ++potential_trump;
        continue;
      }
      return *potential_trump;
    }
    // All trumps are plausible birds.  Play any non-trump if we have one
    if (plays.count(suit)) {
      return *plays.lower_bound(suit);
    }
    assert(bird);
    if (plays.count(*bird)) {
      if (ai.trumps_known_exhausted()) {
        return *bird;
      } else {
        auto something_else = boost::prior(plays.end());
        if (*something_else == *bird) --something_else;
        return *something_else;
      }
    }
    return *boost::prior(plays.end());
  } else {
    // I am following to someone else's trick
    Suit s = trick.suit();

    // See if I can possibly beat what's played so far
    auto winning_play = plays.lower_bound(trick.winning_card());
    if (winning_play != plays.end() && !winning_play->trump() &&
      winning_play->suit() != s) {
      winning_play = plays.end();
    }
    while (winning_play != plays.end() &&
        boost::next(winning_play) != plays.end() &&
        winning_play->trump() &&
        winning_play->trump_rank() <= TrumpRank::kakadu) {
      ++winning_play;
    }

    auto worthless_card = plays.begin();
    while (boost::next(worthless_card) != plays.end() &&
        worthless_card->trump() &&
        worthless_card->trump_rank() < lowest_trump_to_rough_.trump_rank()) {
      ++worthless_card;
    }

    // Make sure the worthless card isn't this trick's bird
    if (bird && *worthless_card == *bird) {
      if (boost::next(worthless_card) != plays.end()) {
        ++worthless_card;
      } else {
        assert(worthless_card != plays.begin());
        --worthless_card;
      }
    }

    if (winning_play == plays.end()) {
      // I can't win; play the least valuable card
      return *worthless_card;
    }

    // If I'm playing a trump and last to play, win minimally
    if (winning_play->trump() && trick.played() == 3) {
      return *winning_play;
    }

    if (s == Suit::trumps) {
      // Defense led trumps; this is a very bad sign
      // Play biggest trump
      return *boost::prior(plays.end());
    } else {
      // Defense led a side suit

      // Try to win with the king
      if (plays.count(Card(s, SuitRank::king))) {
        rippiness_[s] += king_rippiness_penalty_;
        return Card(s, SuitRank::king);
      }

      // Trump first round if I have a void
      if (void_waiting_[s]) {
        void_waiting_[s] = false;
        return *winning_play;
      }

      // If I'm following suit, assume that the king or a trump will win, so
      // play minimally
      if (!worthless_card->trump()) {
        return *worthless_card;
      }

      // If the trick's worth much already, play a big trump
      if (trick.cards_so_far().total_card_points() >= valuable_card_points_) {
        return *boost::prior(plays.end());
      }

      // Otherwise, win minimally
      return *winning_play;
    }

    KONIG_FATAL("not implemented " << trick);
  }
}

}}

