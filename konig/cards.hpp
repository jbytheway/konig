#ifndef KONIG_CARDS_HPP
#define KONIG_CARDS_HPP

#include <set>
#include <algorithm>
#include <array>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/utility/smallset.hpp>
#include <konig/utility/transformingset.hpp>
#include <konig/card.hpp>
#include <konig/cardpoints.hpp>

namespace konig {

namespace cards_detail {

  struct IntToCard {
    Card operator()(Card::index_type const i) const {
      return Card::from_index(i);
    }
  };

  struct CardToInt {
    Card::index_type operator()(Card const c) const {
      return c.index();
    }
  };

  typedef utility::SmallSet<Card::index_type, Card::index_max> CardIntSet;

}

class KONIG_API Cards :
  public utility::TransformingSet<
    cards_detail::CardIntSet,
    cards_detail::IntToCard,
    cards_detail::CardToInt
  > {
  friend class boost::serialization::access;
  public:
    typedef utility::TransformingSet<
      cards_detail::CardIntSet,
      cards_detail::IntToCard,
      cards_detail::CardToInt
    > base_class;

    static Cards make_deck();
    static Cards from_string(std::string const&);
    static bool from_string(Cards&, std::string const&);

    Cards() = default;
    Cards(Cards const&) = default;

    Cards(std::initializer_list<Card> const& list) :
      base_class(list)
    {
    }

    Cards(base_class b) :
      base_class(std::move(b))
    {}

    template<typename Range>
    explicit Cards(Range const& r) {
      std::copy(boost::begin(r), boost::end(r), std::inserter(*this, end()));
    }

    template<typename InputIterator>
    Cards(InputIterator const start, InputIterator const finish) :
      base_class(start, finish)
    {
    }

    using base_class::lower_bound;

    iterator lower_bound(Suit suit) {
      if (suit == Suit::trumps) {
        return lower_bound(Card(TrumpRank::pagat));
      } else {
        return lower_bound(Card(suit, SuitRank::low_pip));
      }
    }

    const_iterator lower_bound(Suit suit) const {
      if (suit == Suit::trumps) {
        return lower_bound(Card(TrumpRank::pagat));
      } else {
        return lower_bound(Card(suit, SuitRank::low_pip));
      }
    }

    using base_class::upper_bound;

    iterator upper_bound(Suit suit) {
      if (suit == Suit::trumps) {
        return upper_bound(Card(TrumpRank::skus));
      } else {
        return upper_bound(Card(suit, SuitRank::king));
      }
    }

    const_iterator upper_bound(Suit suit) const {
      if (suit == Suit::trumps) {
        return upper_bound(Card(TrumpRank::skus));
      } else {
        return upper_bound(Card(suit, SuitRank::king));
      }
    }

    using base_class::equal_range;

    std::pair<iterator, iterator> equal_range(Suit suit) {
      if (suit == Suit::trumps) {
        return std::make_pair(
            lower_bound(Card(TrumpRank::pagat)),
            upper_bound(Card(TrumpRank::skus))
          );
      } else {
        return std::make_pair(
            lower_bound(Card(suit, SuitRank::low_pip)),
            upper_bound(Card(suit, SuitRank::king))
          );
      }
    }

    std::pair<const_iterator, const_iterator> equal_range(Suit suit) const {
      if (suit == Suit::trumps) {
        return std::make_pair(
            lower_bound(Card(TrumpRank::pagat)),
            upper_bound(Card(TrumpRank::skus))
          );
      } else {
        return std::make_pair(
            lower_bound(Card(suit, SuitRank::low_pip)),
            upper_bound(Card(suit, SuitRank::king))
          );
      }
    }

    using base_class::count;

    size_t count(Suit suit) const {
      return subset(suit).size();
    }

    size_t count(TrumpRank rank) const {
      return count(Card(rank));
    }

    size_t count(SuitRank rank) const {
      size_t tally = 0;
      for (Suit s = Suit::min; s<Suit::trumps; ++s) {
        tally += count(Card(s, rank));
      }
      return tally;
    }

    using base_class::find;

    iterator find(const Suit suit) {
      return std::find_if(
          begin(), end(), boost::bind(&Card::suit, _1) == suit
        );
    }

    const_iterator find(const Suit suit) const {
      return std::find_if(
          begin(), end(), boost::bind(&Card::suit, _1) == suit
        );
    }

    iterator find(const SuitRank rank) {
      return std::find_if(
          begin(), end(),
          !boost::bind(&Card::trump, _1) &&
            boost::bind(&Card::suit_rank, _1) == rank
        );
    }

    using base_class::insert;

    template<typename Range>
    void insert(Range const& r) {
      std::copy(boost::begin(r), boost::end(r), std::inserter(*this, end()));
    }

    using base_class::erase;

    void erase(const Cards& to_erase) {
      BOOST_FOREACH(const Card& c, to_erase) {
        erase(c);
      }
    }

    void erase(const Suit s) {
      erase(lower_bound(s), upper_bound(s));
    }

    void erase(SuitRank rank) {
      for (Suit s = Suit::min; s<Suit::trumps; ++s) {
        erase(Card(s, rank));
      }
    }

    void erase(TrumpRank rank) {
      erase(Card(rank));
    }

    using base_class::subset;

    Cards subset(Suit const s) const {
      Card min, max;

      if (s == Suit::trumps) {
        return {subset_after(Card(TrumpRank::min))};
      } else {
        return {
          subset(Card(s), Card(Suit(s+1)))
        };
      }
    }

    bool contains(const Cards& c) const {
      Cards i(c);
      intersection_into(i, *this);
      return i == c;
    }

    CardPoints total_card_points() const;
  private:
    template<typename Archive>
    void serialize(Archive& archive, unsigned int) {
      archive & boost::serialization::make_nvp(
          "set", boost::implicit_cast<base_class&>(*this)
        );
    }
};

KONIG_API std::ostream& operator<<(std::ostream&, const Cards&);

// HACK: This should go once boost::serialization supports std::array natively
template<typename Archive, size_t N>
void serialize(Archive& ar, std::array<Cards, N>& cards, const unsigned int)
{
  BOOST_FOREACH(Cards& card, cards) {
    ar & boost::serialization::make_nvp("array_mem", card);
  }
}

}

#endif // KONIG_CARDS_HPP

