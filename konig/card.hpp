#ifndef KONIG__CARD_HPP
#define KONIG__CARD_HPP

#include <cassert>

#include <konig/suit.hpp>
#include <konig/trumprank.hpp>
#include <konig/suitrank.hpp>

namespace konig {

class Card {
  public:
    template<typename OutputIterator>
    static void make_deck(OutputIterator);

    explicit Card(const unsigned long r) : suit_(Suit::trumps), rank_(r) {
      static_cast<void>(TrumpRank(r)); // construction checks validity
    }
    explicit Card(const TrumpRank r) : suit_(Suit::trumps), rank_(r) {
      assert(r.valid());
    }
    Card(const Suit s, const SuitRank r) : suit_(s), rank_(r) {
      assert(s.valid());
      assert(r.valid());
      assert(!trump());
    }

    bool trump() const { return suit_ == Suit::trumps; }

    bool operator==(const Card& r) const {
      return rank_ == r.rank_ && suit_ == r.suit_;
    }
    bool operator<(const Card& r) const {
      return suit_ < r.suit_ || (suit_ == r.suit_ && rank_ < r.rank_);
    }
  private:
    uint8_t suit_;
    uint8_t rank_;
};

template<typename OutputIterator>
void Card::make_deck(OutputIterator o) {
  for (Suit s = Suit::min; s < Suit::trumps; ++s) {
    for (SuitRank r = SuitRank::low_pip; r <= SuitRank::king; ++r) {
      *o = Card(s, r);
      ++o;
    }
  }
  for (TrumpRank r = TrumpRank::pagat; r <= TrumpRank::skus; ++r) {
    *o = Card(r);
    ++o;
  }
}

}

#endif // KONIG__CARD_HPP

