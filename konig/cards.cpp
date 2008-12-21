#include <konig/cards.hpp>

#include <algorithm>

#include <boost/bind.hpp>
#include <boost/range/empty.hpp>

namespace konig {

std::ostream& operator<<(std::ostream& o, const Cards& c)
{
  typedef std::pair<Cards::const_iterator, Cards::const_iterator> Range;
  typedef Cards::const_reverse_iterator reverse_iterator;
  typedef std::pair<reverse_iterator, reverse_iterator> ReverseRange;
  ReverseRange trumps = c.equal_range(Suit::trumps);
  std::transform(
      trumps.second, trumps.first, std::ostream_iterator<TrumpRank>(o, " "),
      boost::bind(&Card::trump_rank, _1)
    );

  for (Suit s = Suit::min; s<Suit::trumps; ++s) {
    ReverseRange suit = c.equal_range(s);
    if (!boost::empty(suit)) {
      o << s << ':';
      std::transform(
          suit.second, suit.first, std::ostream_iterator<SuitRank>(o),
          boost::bind(&Card::suit_rank, _1)
        );
      o << ' ';
    }
  }
  return o;
}

}

