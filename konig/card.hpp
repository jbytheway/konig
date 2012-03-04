#ifndef KONIG_CARD_HPP
#define KONIG_CARD_HPP

#include <cassert>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/api.hpp>
#include <konig/suit.hpp>
#include <konig/trumprank.hpp>
#include <konig/suitrank.hpp>

namespace konig {

class KONIG_API Card {
  friend class boost::serialization::access;
  public:
    template<typename OutputIterator>
    static void make_deck(OutputIterator);

    static bool from_string(Card&, std::string const&);

    struct CompareSuitRanks {
      bool operator()(const Card& l, const Card& r) const {
        return l.suit_rank() < r.suit_rank();
      }
    };

    // Like CompareRanks, but reverse the order of the pip cards (undefined
    // behaviour on trumps)
    struct CompareRanksReversePips {
      bool operator()(const Card& l, const Card& r) const {
        SuitRank const lRank = l.suit_rank();
        SuitRank const rRank = r.suit_rank();
        if (lRank.face()) {
          if (rRank.face()) {
            return lRank < rRank;
          } else {
            return false;
          }
        } else {
          if (rRank.face()) {
            return true;
          } else {
            return rRank < lRank;
          }
        }
      }
    };

    Card() : suit_(Suit::trumps), rank_(TrumpRank::pagat) {}

    explicit Card(Suit const s) :
      suit_(s),
      rank_(s == Suit::trumps ?
        rank_type(TrumpRank::min) : rank_type(SuitRank::min))
    {
      assert(s.valid());
    }

    explicit Card(const TrumpRank r) :
      suit_(Suit::trumps),
      rank_(r)
    {
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

    unsigned int card_points() const;

    bool operator==(const Card& r) const {
      return rank_ == r.rank_ && suit_ == r.suit_;
    }
    bool operator<(const Card& r) const {
      return suit_ < r.suit_ || (suit_ == r.suit_ && rank_ < r.rank_);
    }
    bool operator>(const Card& r) const {
      return r < *this;
    }
  private:
    typedef uint8_t rank_type;

    template<typename Archive>
    void serialize(Archive& ar, unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(suit_);
      ar & BOOST_SERIALIZATION_NVP(rank_);
    }

    uint8_t suit_;
    rank_type rank_;
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

std::ostream& operator<<(std::ostream& o, const Card& c);

}

#endif // KONIG_CARD_HPP

