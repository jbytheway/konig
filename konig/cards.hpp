#ifndef KONIG__CARDS_HPP
#define KONIG__CARDS_HPP

#include <set>
#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
//#include <boost/spirit/home/phoenix/operator/member.hpp>
//#include <boost/spirit/home/phoenix/operator/comparison.hpp>

#include <konig/card.hpp>

namespace konig {

class Cards : public std::set<Card> {
  public:
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

    size_t count(Suit suit) {
      std::pair<iterator, iterator> range = equal_range(suit);
      return std::distance(range.first, range.second);
    }

    size_t count(TrumpRank rank) {
      return count(Card(rank));
    }

    size_t count(SuitRank rank) {
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

    void insert(const Cards& to_insert) {
      copy(to_insert.begin(), to_insert.end(), inserter(*this, end()));
    }

    template<size_t size>
    void insert(const boost::array<Card, size>& to_insert) {
      copy(to_insert.begin(), to_insert.end(), inserter(*this, end()));
    }

    using std::set<Card>::erase;

    void erase(const Cards& to_erase) {
      BOOST_FOREACH(const Card& c, to_erase) {
        erase(c);
      }
    }

    bool contains(const Cards& c) {
      BOOST_FOREACH(const Card card, c) {
        if (!count(card)) return false;
      }
      return true;
    }

    unsigned int total_card_points() const {
      unsigned int tally = 0;
      BOOST_FOREACH(const Card& card, *this) {
        tally += card.card_points();
      }
      return tally;
    }
};

std::ostream& operator<<(std::ostream&, const Cards&);

}

#endif // KONIG__CARDS_HPP

