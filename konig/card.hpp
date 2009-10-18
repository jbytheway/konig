#ifndef KONIG__CARD_HPP
#define KONIG__CARD_HPP

#include <cassert>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/suit.hpp>
#include <konig/trumprank.hpp>
#include <konig/suitrank.hpp>

namespace konig {

class Card {
  friend class boost::serialization::access;
  public:
    template<typename OutputIterator>
    static void make_deck(OutputIterator);

    struct CompareRanks {
      bool operator()(const Card& l, const Card& r) {
        return l.rank_ < r.rank_;
      }
    };

    Card() : suit_(Suit::trumps), rank_(TrumpRank::pagat) {}
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
    Card(const std::string&);

    bool trump() const { return suit_ == Suit::trumps; }

    Suit suit() const { return Suit(Suit::internal_enum(suit_)); }

    TrumpRank trump_rank() const {
      assert(trump()); return TrumpRank(TrumpRank::internal_enum(rank_));
    }

    SuitRank suit_rank() const {
      assert(!trump()); return SuitRank(SuitRank::internal_enum(rank_));
    }

    unsigned int card_points() const {
      if (trump()) {
        if (rank_ == TrumpRank::pagat ||
            rank_ == TrumpRank::mond ||
            rank_ == TrumpRank::skus)
          return 13;
        return 1;
      } else {
        switch (rank_) {
          case SuitRank::king:
            return 13;
          case SuitRank::queen:
            return 10;
          case SuitRank::knight:
            return 7;
          case SuitRank::jack:
            return 4;
          default:
            return 1;
        }
      }
    }

    bool operator==(const Card& r) const {
      return rank_ == r.rank_ && suit_ == r.suit_;
    }
    bool operator<(const Card& r) const {
      return suit_ < r.suit_ || (suit_ == r.suit_ && rank_ < r.rank_);
    }
  private:
    template<typename Archive>
    void serialize(Archive& ar, unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(suit_);
      ar & BOOST_SERIALIZATION_NVP(rank_);
    }

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

inline std::ostream& operator<<(std::ostream& o, const Card& c) {
  if (c.trump()) {
    o << c.trump_rank();
  } else {
    o << c.suit() << c.suit_rank();
  }
  return o;
}

}

#endif // KONIG__CARD_HPP

