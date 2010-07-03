#include <konig/ai/roffenceai.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace ai {

void ROffenceAi::reset(FateAi const&)
{
}

KingCall ROffenceAi::call_king(FateAi const& ai)
{
  auto const& hand = ai.hand();
  assert(hand.size() == 12);
  // Find smallest nonempty suit, and call that
  // TODO: pick between suits of equal size better
  std::array<size_t, Suit::trumps> suit_scores;
  for (Suit s = Suit::min; s < Suit::trumps; ++s) {
    size_t size = hand.count(s);
    suit_scores[s] = size ? size : 13;
  }
  size_t to_call =
    std::min_element(suit_scores.begin(), suit_scores.end()) -
    suit_scores.begin();
  // TODO: consider calling fourth king
  return KingCall::from_value(to_call);
}

uint8_t ROffenceAi::choose_talon_half(FateAi const& ai)
{
  auto const& talon = ai.talon();
#define TAKE_IF_GREATER(pre, post) \
  do {                             \
    auto score0 = pre 0 post;      \
    auto score1 = pre 1 post;      \
    if (score0 > score1) {         \
      return 0;                    \
    } else if (score0 < score1) {  \
      return 1;                    \
    }                              \
  } while(false)
  // Take half with more trumps
  TAKE_IF_GREATER(talon[,].count(Suit::trumps));
  // Take half with more kings
  TAKE_IF_GREATER(talon[,].count(SuitRank::king));
  // Take half with more points
  TAKE_IF_GREATER(talon[,].total_card_points());
  // TODO: worry about shape
  // just take the first half
  return 0;
}

Cards ROffenceAi::discard(FateAi const&)
{
  KONIG_FATAL("not implemented in " << typeid(*this).name());
}

std::vector<Announcement> ROffenceAi::announce(FateAi const&)
{
  return std::vector<Announcement>();
}

Card ROffenceAi::play_card(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  //Cards const& hand = ai.hand();
  //Cards const plays = ai.legal_plays();

  KONIG_FATAL("not implemented " << trick);
}

}}

