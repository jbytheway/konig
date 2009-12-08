#include <konig/ai/sddefenceai.hpp>

#include <tuple>

#include <boost/range/size.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>

namespace konig { namespace ai {

void SdDefenceAi::play_start(FateAi const& ai)
{
  sizes_of_suits_.clear();
  cards_to_preserve_.clear();
  // collect size and value of each suit
  std::map<Suit, std::pair<size_t, size_t>> suits;
  BOOST_FOREACH(const Card& card, ai.hand()) {
    std::map<Suit, std::pair<size_t, size_t>>::iterator i =
      suits.insert(std::make_pair(card.suit(), std::make_pair(0, 0))).first;
    i->second.first += 1;
    i->second.second += card.card_points();
  }
  // Invert the map so we know what our shortest suits are
  typedef std::pair<Suit, std::pair<size_t, size_t>> SuitSizeAndPoints;
  BOOST_FOREACH(const SuitSizeAndPoints& s, suits) {
    sizes_of_suits_.push_back(
        std::make_tuple(s.second.first, s.second.second, s.first)
      );
  }
  std::sort(sizes_of_suits_.begin(), sizes_of_suits_.end());
  longest_suit_ = Suit::trumps;

  for (SizesOfSuits::iterator i = sizes_of_suits_.begin();
      i != sizes_of_suits_.end(); ++i) {
    Suit suit = std::get<2>(*i);
    if (suit == Suit::trumps) {
      continue;
    }
    const auto suit_range = ai.hand().equal_range(suit);
    SuitRank top_rank = boost::prior(suit_range.second)->suit_rank();
    int num_needed = SuitRank::king - top_rank + 1;
    if (std::distance(suit_range.first, suit_range.second) < num_needed) {
      continue;
    }
    Cards::iterator preserve = suit_range.second;
    while (num_needed--) {
      cards_to_preserve_.insert(*--preserve);
    }
    longest_suit_ = suit;
  }
}

std::vector<Announcement> SdDefenceAi::announce(FateAi const&) {
  return std::vector<Announcement>();
}

Card SdDefenceAi::play_card(FateAi const& ai) {
  Cards legal = ai.legal_plays();
  const Trick& trick = ai.tricks().back();
  Cards unpreserved;
  Cards preserved;
  // Slower than using proper algorithms...
  BOOST_FOREACH(const Card& c, legal) {
    if (cards_to_preserve_.count(c)) {
      preserved.insert(c);
    } else {
      unpreserved.insert(c);
    }
  }

  if (trick.played() == 0) {
    // We're leading
    return play_low_short(unpreserved, preserved);
  }
  const int declarers_position_in_trick = (ai.declarer() - trick.leader() + 4)%4;
  const bool declarer_has_played = trick.played() > declarers_position_in_trick;
  const Card declarer_played = trick.cards()[declarers_position_in_trick];
  const bool declarer_played_strongly =
    declarer_has_played &&
    (declarer_played.trump() || declarer_played.suit_rank() == SuitRank::king);
  std::set<Suit> suits;
  std::transform(
      legal.begin(), legal.end(), inserter(suits, suits.end()),
      boost::bind(&Card::suit, _1)
    );
  if (suits.size() == 1) {
    // We're either following suit or trumping or discarding from our only suit
    if (declarer_played_strongly) {
      // Play low
      return *legal.begin();
    } else {
      // Play high
      return *boost::prior(legal.end());
    }
  } else {
    // We're discarding
    if (declarer_played_strongly && trick.winner() == ai.declarer()) {
      // Discard bottom card from shortest suit
      return play_low_short(unpreserved, preserved);
    } else {
      // Discard most valuable card from unpreserved things
      std::vector<Card> available;
      std::copy(
          unpreserved.begin(), unpreserved.end(), back_inserter(available)
        );
      if (available.empty()) {
        std::copy(
            preserved.begin(), preserved.end(), back_inserter(available)
          );
      }
      std::sort(available.begin(), available.end(), Card::CompareRanks());
      return available.back();
    }
  }
}

Card SdDefenceAi::play_low_short(
    const Cards& unpreserved,
    const Cards& preserved
  ) const
{
  // Play a low card from a short suit
  typedef std::tuple<size_t, size_t, Suit> SizePointsAndSuit;
  BOOST_FOREACH(const SizePointsAndSuit& s, sizes_of_suits_) {
    Cards::iterator i = unpreserved.find(std::get<2>(s));
    if (i != unpreserved.end()) {
      return *i;
    }
  }
  BOOST_FOREACH(const SizePointsAndSuit& s, sizes_of_suits_) {
    Cards::iterator i = preserved.find(std::get<2>(s));
    if (i != preserved.end()) {
      return *i;
    }
  }
  throw std::logic_error("shouldn't get here");
}

}}

