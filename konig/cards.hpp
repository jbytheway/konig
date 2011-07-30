#ifndef KONIG_CARDS_HPP
#define KONIG_CARDS_HPP

#include <set>
#include <algorithm>
#include <array>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/card.hpp>
#include <konig/cardpoints.hpp>

namespace konig {

class KONIG_API Cards : public std::set<Card> {
  friend class boost::serialization::access;
  public:
    static Cards from_string(std::string const&);
    static bool from_string(Cards&, std::string const&);

    Cards() = default;
    Cards(Cards const&) = default;

    Cards(std::initializer_list<Card> const& list) :
      std::set<Card>(list)
    {
    }

    template<typename Range>
    explicit Cards(Range const& r) {
      std::copy(boost::begin(r), boost::end(r), std::inserter(*this, end()));
    }

    template<typename InputIterator>
    Cards(InputIterator const start, InputIterator const finish) :
      std::set<Card>(start, finish)
    {
    }

    using std::set<Card>::lower_bound;

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

    using std::set<Card>::upper_bound;

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

    using std::set<Card>::equal_range;

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

    using std::set<Card>::count;

    size_t count(Suit suit) const {
      std::pair<iterator, iterator> range = equal_range(suit);
      return std::distance(range.first, range.second);
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

    using std::set<Card>::find;

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

    using std::set<Card>::insert;

    template<typename Range>
    void insert(Range const& r) {
      std::copy(boost::begin(r), boost::end(r), std::inserter(*this, end()));
    }

    using std::set<Card>::erase;

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

    bool contains(const Cards& c) {
      BOOST_FOREACH(const Card card, c) {
        if (!count(card)) return false;
      }
      return true;
    }

    CardPoints total_card_points() const {
      CardPoints tally = 0;
      BOOST_FOREACH(const Card& card, *this) {
        tally += card.card_points();
      }
      return tally;
    }
  private:
    template<typename Archive>
    void serialize(Archive& archive, unsigned int) {
      archive & boost::serialization::make_nvp(
          "set", boost::implicit_cast<std::set<Card>&>(*this)
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

