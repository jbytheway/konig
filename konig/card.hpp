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
    typedef uint8_t index_type;

    template<typename OutputIterator>
    static void make_deck(OutputIterator);

    static bool from_string(Card&, std::string const&);

    static constexpr index_type index_first_trump = 8*4;
    static constexpr index_type index_max = 54;

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

    Card() : index_(index_first_trump) {}

    static Card from_index(index_type i) {
      return Card(i, construct_from_index());
    }

    explicit Card(Suit const s) :
      index_(s * 8)
    {
      assert(s.valid());
    }

    explicit Card(const TrumpRank r) :
      index_(r + index_first_trump - TrumpRank::min)
    {
      assert(r.valid());
    }

    Card(const Suit s, const SuitRank r) : index_(s * 8 + r - SuitRank::min) {
      assert(s.valid());
      assert(r.valid());
      assert(!trump());
    }

    Card(const std::string&);

    index_type index() const { return index_; }

    bool trump() const { return index_ >= index_first_trump; }

    Suit suit() const {
      if (trump()) {
        return Suit::trumps;
      } else {
        return Suit(Suit::internal_enum(index_ / 8));
      }
    }

    TrumpRank trump_rank() const {
      assert(trump());
      return TrumpRank(TrumpRank::internal_enum(
          index_ - index_first_trump + TrumpRank::min
        ));
    }

    SuitRank suit_rank() const {
      assert(!trump());
      return SuitRank(SuitRank::internal_enum(index_ % 8 + SuitRank::min));
    }

    unsigned int card_points() const;

    bool operator==(const Card& r) const {
      return index() == r.index();
    }
    bool operator<(const Card& r) const {
      return index() < r.index();
    }
    bool operator>(const Card& r) const {
      return r < *this;
    }
  private:
    struct construct_from_index {};
    explicit Card(index_type const i, construct_from_index) : index_(i) {
      assert(i < index_max);
    }

    template<typename Archive>
    void serialize(Archive& ar, unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(index_);
    }

    index_type index_;
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

