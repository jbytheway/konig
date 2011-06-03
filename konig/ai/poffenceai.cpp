#include <konig/ai/poffenceai.hpp>

#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/algorithm/min_element.hpp>

#include <konig/ai/get_dangerous_suit_candidates.hpp>
#include <konig/ai/negative_offensive.hpp>

namespace konig { namespace ai {

void POffenceAi::reset(FateAi const& ai)
{
  Cards const& hand = ai.hand();
  target_cards_.clear();
  if (hand.count(TrumpRank::skus)) {
    target_cards_.insert(Card(TrumpRank::skus));
    // If we're going for the Skus, then we don't want to try for anything else
    return;
  }
  // Score each card for how likely we are to win with it (purely arbitrary
  // heuristic scoring)
  std::vector<std::pair<int, Card>> candidates;
  BOOST_FOREACH(Card const& card, hand) {
    // Balance it to make the mond as good a chance as the singleton queen
    if (card.trump()) {
      candidates.push_back({card.trump_rank() - TrumpRank::mond - 1, card});
    } else {
      auto const suit_size = hand.count(card.suit());
      candidates.push_back(
        {card.suit_rank() - SuitRank::queen - suit_size, card}
      );
    }
  }
  assert(candidates.size() >= 2);
  // Take the top two and mark them as what we want to try for
  std::nth_element(candidates.begin(), candidates.end()-2, candidates.end());
  std::for_each(
    candidates.end()-2, candidates.end(),
    [this](std::pair<int, Card> const& p) { target_cards_.insert(p.second); }
  );
  assert(target_cards_.size() == 2);
}

Card POffenceAi::play_card(FateAi const& ai)
{
  Trick const& trick = ai.tricks().back();
  Cards const& hand = ai.hand();
  Cards const plays = ai.legal_plays();

  // If there's only one legal play, play it
  if (plays.size() == 1) {
    return *plays.begin();
  }

  // If I've already taken two tricks, then it doesn't matter; play anything
  // legal
  auto const num_tricks_taken = ai.num_tricks_taken_by(ai.position());
  if (num_tricks_taken > 1) {
    return *plays.begin();
  }

  // Check to see whether all legal plays are equivalent; if so play whatever.
  if (ai.cards_are_equivalent(plays)) {
    return *plays.begin();
  }

  if (num_tricks_taken == 1) {
    // I've taken my one trick; now I'm playing purely negatively
    return negative_offensive(ai);
  }

  if (trick.leader() == ai.position()) {
    // I am leading to the trick (note this happens at most twice, on the first
    // trick or after I win)

    {
      // This function picks out good plays in side suits
      auto candidates = get_dangerous_suit_candidates(ai);

      // But delete candidates which would spoil our targets
      erase_bad_candidates(
        candidates, [](std::pair<int, Card> const& p){ return p.second; }
      );

      // We should also consider trumps
      auto big_trump = boost::prior(plays.end());
      if (target_cards_.count(*big_trump)) {
        --big_trump;
      }
      if (big_trump->trump()) {
        // This makes the mond as bad as a singleton queen.
        int score = big_trump->trump_rank() - TrumpRank::mond + 7;
        candidates.push_back({score, *big_trump});
      }

      // Having gathered all the candidates, play the best one
      if (!candidates.empty()) {
        auto it = boost::range::max_element(candidates);
        return it->second;
      }
    }

    // Nothing clever to do, lets just play anything small doesn't mess with
    // the targets
    std::vector<Card> candidates(plays.begin(), plays.end());
    erase_bad_candidates(candidates, [](Card const& c){ return c; });
    if (!candidates.empty()) {
      return *boost::range::min_element(candidates, Card::CompareRanks());
    }
  } else {
    // I am following
    Suit s = trick.suit();

    bool const will_rise =
      (plays.begin()->trump() && s != Suit::trumps) ||
      (plays.begin()->suit() == s && *plays.begin() > trick.winning_card());

    if (will_rise) {
      // If we can play one of our target cards, we should
      BOOST_FOREACH(auto const& target, target_cards_) {
        if (plays.count(target)) {
          return target;
        }
      }

      // Otherwise, play low
      return *plays.begin();
    }

    // Will not rise; play most dangerous card that won't interfere with our
    // target plays
    std::vector<Card> candidates(plays.begin(), plays.end());
    erase_bad_candidates(candidates, [](Card const& c){ return c; });
    if (!candidates.empty()) {
      return *boost::range::max_element(candidates, Card::CompareRanks());
    }

    // Every play is potentially interfereing with our targets
    // TODO: this is probably the endgame, and we should really consider it
    // carefully; we might know something about which suits are held, etc.
    // But for now, be lazy and just play low (low rather than high because
    // there are probably few tricks left and we need to win one!  also, it
    // helps retain our targets).
    return *boost::range::min_element(plays, Card::CompareRanks());
  }
  KONIG_FATAL("not implemented\ntrick " << trick << " hand " << hand);
}

template<typename Container, typename F>
void POffenceAi::erase_bad_candidates(Container& candidates, F const& f)
{
  for (size_t i = 0; i < candidates.size(); ) {
    Card const c = f(candidates[i]);
    if (target_cards_.count(c) ||
          (!c.trump() && target_cards_.count(Card(c.suit(), SuitRank::king)))
        ) {
      candidates.erase(candidates.begin()+i);
    } else {
      ++i;
    }
  }
}

}}

