#include <konig/ai/sdoffenceai.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace ai {

SdOffenceAi::SdOffenceAi()
{
}

std::vector<Announcement> SdOffenceAi::announce(Ai const&)
{
  return std::vector<Announcement>();
}

void SdOffenceAi::play_start(Ai const& ai)
{
  num_voids_ = 0;
  for (Suit s = Suit::min; s<Suit::trumps; ++s) {
    Cards in_suit(ai.hand().equal_range(s));
    num_voids_ += in_suit.empty();
    rippiness_[s] = in_suit.size();
    if (in_suit.count(SuitRank::king)) {
      rippiness_[s] -= 10;
    }
  }
  // Get non-bird trumps
  Cards trumps(ai.hand().upper_bound(Card(TrumpRank::kakadu)), ai.hand().end());
  if (trumps.size() <= num_voids_) {
    lowish_trump_ = Card(TrumpRank::skus);
  } else {
    lowish_trump_ = *boost::next(trumps.begin(), num_voids_);
  }
}

Card SdOffenceAi::play_card(Ai const& ai)
{
  Trick const& trick = ai.tricks().back();
  if (trick.leader() == ai.position()) {
    // I am leading to the trick
    // First try ripping in a side suit
    auto suit_it = std::max_element(rippiness_.begin(), rippiness_.end());
    if (*suit_it >= 3) {
      Suit s(suit_it-rippiness_.begin());
      assert(ai.hand().count(s));
      return *ai.hand().lower_bound(s);
    }
  }
  KONIG_FATAL("not implemented");
}

}}

