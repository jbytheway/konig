#include <konig/ai/minimalannouncementai.hpp>

#include <numeric>

#include <boost/spirit/home/phoenix/operator/member.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>

#include <konig/fatal.hpp>
#include <konig/ai/suitprofile.hpp>

namespace konig { namespace ai {

void MinimalAnnouncementAi::reset(FateAi const& ai)
{
  AnnouncementAi::reset(ai);
  first_announcements_ = true;
}

bool MinimalAnnouncementAi::choose_concede(FateAi const&)
{
  // Never give up; never surrender!
  return false;
}

KingCall MinimalAnnouncementAi::call_king(FateAi const& ai)
{
  auto const& hand = ai.hand();
  assert(hand.size() == 12);
  // Find smallest nonempty suit in which we don't hold a king, and call that
  // TODO: pick between suits of equal size better
  std::array<size_t, Suit::trumps> suit_scores;
  for (Suit s = Suit::min; s < Suit::trumps; ++s) {
    size_t const size = hand.count(s);
    Card const king(s, SuitRank::king);
    bool const have_king = hand.count(king);
    suit_scores[s] = size ? ( have_king ? 14 : size ) : 13;
  }
  size_t to_call =
    std::min_element(suit_scores.begin(), suit_scores.end()) -
    suit_scores.begin();
  // TODO: consider calling fourth king
  return KingCall::from_value(to_call);
}

uint8_t MinimalAnnouncementAi::choose_talon_half(FateAi const& ai)
{
  auto const& talon = ai.talon();
#define TAKE_IF_GREATER(pre, post)  \
  do {                              \
    auto const score0 = pre 0 post; \
    auto const score1 = pre 1 post; \
    if (score0 > score1) {          \
      return 0;                     \
    } else if (score0 < score1) {   \
      return 1;                     \
    }                               \
  } while(false)
  // TODO: worry about when hand is already very trump-full
  // Take half with more trumps
  TAKE_IF_GREATER(talon[,].count(Suit::trumps));
  // TODO: worry about quality of trumps
  // Take half with more kings
  TAKE_IF_GREATER(talon[,].count(SuitRank::king));
  // Take half with more points
  TAKE_IF_GREATER(talon[,].total_card_points());
  // TODO: worry about shape
  // TODO: randomize?
  // just take the first half
  return 0;
}

Cards MinimalAnnouncementAi::discard(FateAi const& ai)
{
  Cards hand = ai.hand();
  Cards const& accepted = ai.accepted();
  std::copy(
    accepted.begin(), accepted.end(),
    std::inserter(hand, hand.end())
  );
  assert(hand.size() == 15 || hand.size() == 18);
  size_t const num_to_discard = hand.size() - 12;

  Cards promising_cards(hand);
  promising_cards.erase(Suit::trumps);
  // Probably don't want to discard from a suit in which we hold the king, or
  // which we called
  for (Suit s = Suit::min; s < Suit::trumps; ++s) {
    if (s == ai.called_suit(Suit::trumps/*fallback for non-partnership*/) ||
        promising_cards.count(Card(s, SuitRank::king))) {
      promising_cards.erase(s);
    }
  }
  // Now we want to maximise the number of voids we can get
  auto profiles = SuitProfile::make_profiles(promising_cards);
  std::vector<size_t> sizes(profiles.size());
  std::transform(
    profiles.begin(), profiles.end(), sizes.begin(),
    (&arg1)->*&SuitProfile::size
  );
  std::vector<size_t> cumulative(profiles.size());
  std::partial_sum(sizes.begin(), sizes.end(), cumulative.begin());
  assert(cumulative.empty() || cumulative[0] > 0);
  auto finish = std::find_if(
    cumulative.begin(), cumulative.end(), arg1 > num_to_discard);
  Cards discard;
  for (size_t i=0; i<size_t(finish-cumulative.begin()); ++i) {
    discard.insert(promising_cards.equal_range(profiles[i].suit));
  }
  assert(discard.size() <= num_to_discard);
  if (discard.size() == num_to_discard) return discard;
  // We've added as many voids as possible, but still need to discard more
  // cards.  So we'll discard the most valuable cards we're allowed to
  {
    Cards available = hand;
    available.erase(discard);
    available.erase(Suit::trumps);
    available.erase(SuitRank::king);

    std::vector<Card> best_options(available.begin(), available.end());
    std::sort(
      best_options.begin(), best_options.end(),
      Card::CompareRanksReversePips()
    );
    while (discard.size() < num_to_discard && !best_options.empty()) {
      discard.insert(best_options.back());
      best_options.pop_back();
    }
  }
  assert (discard.size() <= num_to_discard);
  if (discard.size() < num_to_discard) {
    // We must discard some trumps
    Cards available = hand;
    available.erase(discard);
    available.erase(TrumpRank::pagat);
    available.erase(TrumpRank::mond);
    available.erase(TrumpRank::skus);
    available.erase(SuitRank::king);

    while (discard.size() < num_to_discard) {
      auto best = available.upper_bound(Card(TrumpRank::kakadu));
      if (best == available.end()) {
        // We are forced to discard a bird; prefer uhu
        // (this is actually impossible, but whatever...)
        best = available.begin();
      }
      discard.insert(*best);
      available.erase(*best);
    }
  }
  return discard;
}

std::vector<Announcement> MinimalAnnouncementAi::announce(FateAi const& ai)
{
  auto contract_and_announcements = ai.contract();
  if (first_announcements_) {
    first_announcements_ = false;
    if (contract_and_announcements.contract().must_announce_bird() &&
      ai.position() == ai.declarer()) {
      auto const& hand = ai.hand();
      auto const first_bird = hand.lower_bound(Suit::trumps);
      Feat feat = Feat::pagat;
      if (first_bird != hand.end() &&
        first_bird->trump_rank() <= TrumpRank::kakadu) {
        feat = Feat(first_bird->trump_rank());
      }
      return {{feat, Announcedness::announced, false}};
    }
  }
  return {};
}

}}

