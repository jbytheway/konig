#include <konig/ai/sdoffenceai.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace ai {

SdOffenceAi::SdOffenceAi()
{
}

std::vector<Announcement> SdOffenceAi::announce(FateAi const&)
{
  return std::vector<Announcement>();
}

void SdOffenceAi::play_start(FateAi const& ai)
{
  num_voids_ = 0;
  for (Suit s = Suit::min; s<Suit::trumps; ++s) {
    Cards in_suit(ai.hand().equal_range(s));
    num_voids_ += in_suit.empty();
    void_waiting_[s] = in_suit.empty();
    rippiness_[s] = in_suit.size();
    if (in_suit.count(SuitRank::king)) {
      rippiness_[s] -= 10;
    }
  }
  // Determine which birds are even vaguely worth trying for
  TrumpRank first_non_bird(4);
  // Following four lines ook good but in fact make things worse for now
  //size_t num_trumps = ai.hand().count(Suit::trumps);
  //if (num_trumps < 5) --first_non_bird;
  //if (num_trumps < 6) --first_non_bird;
  //if (num_trumps < 7) --first_non_bird;
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

Card SdOffenceAi::play_card(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();

  // If there's only one legal play, play it
  if (plays.size() == 1) {
    return *plays.begin();
  }

  if (trick.leader() == ai.position()) {
    // I am leading to the trick

    // Abort Pagat on Uhu trick if there are at least 2 trumps unaccounted for
    if (plays.count(Card(TrumpRank::pagat)) && ai.tricks().size() == 11 &&
          ai.trumps_out().size() >= 2) {
      return Card(TrumpRank::pagat);
    }

    // Cash a bird if it's safe to
    boost::optional<Card> bird = ai.relevant_bird();
    if (bird && plays.count(*bird) &&
          ai.trumps_known_exhausted()) {
      return *bird;
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
    if (*suit_it >= 3 || hand.count(Suit::trumps)*2 <= hand.size()) {
      // Make more rippy so we're inclined to continue on the same suit
      ++*suit_it;
      assert(plays.count(suit));
      return *plays.lower_bound(suit);
    }
    // No worthwhile ripping suits, so lead trumps
    auto potential_trump = hand.lower_bound(lowest_trump_to_lead_);
    if (potential_trump == hand.end()) {
      // We've exhausted all trumps expect plausible birds and those saved for
      // roughing.  Look for one of the latter
      potential_trump = hand.lower_bound(lowest_trump_to_rough_);
    }
    if (potential_trump != hand.end()) {
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
        rippiness_[s] += 10;
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
      if (trick.cards_so_far().total_card_points() >= 10) {
        return *boost::prior(plays.end());
      }

      // Otherwise, win minimally
      return *winning_play;
    }

    KONIG_FATAL("not implemented " << trick);
  }
}

}}

