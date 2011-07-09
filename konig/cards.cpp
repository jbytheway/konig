#include <konig/cards.hpp>

#include <algorithm>

#include <boost/bind.hpp>
#include <boost/range/empty.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/stl/container.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_operator.hpp>

namespace konig {

Cards Cards::from_string(std::string const& description)
{
  namespace qi = boost::spirit::qi;
  auto first = description.begin();
  auto const last = description.end();
  Suit temp;
  std::vector<Card> result;
  bool x = qi::phrase_parse(
      first,
      last,
      *(
        qi::uint_[px::push_back(
            px::ref(result),
            px::construct<Card>(px::construct<TrumpRank>(qi::_1))
          )] |
        qi::lit("Sk")[px::push_back(px::ref(result), Card(TrumpRank::skus))] |
        (
          (qi::char_('C') | qi::char_('D') | qi::char_('H') | qi::char_('S'))[
              px::ref(temp) = px::bind(&Suit::from_char, qi::_1)
            ] >>
          qi::lit(':') >>
          *(
            qi::char_('K') | qi::char_('Q') | qi::char_('N') | qi::char_('J') |
            qi::char_('t') | qi::char_('9') | qi::char_('8') | qi::char_('7') |
            qi::char_('1') | qi::char_('2') | qi::char_('3') | qi::char_('4')
          )[
              px::push_back(
                px::ref(result),
                px::construct<Card>(
                  px::ref(temp),
                  px::bind(&SuitRank::from_char, qi::_1)
                )
              )
            ]
        )
      ),
      qi::space | qi::char_('_')
    );
  if (!x || first != last) {
    throw std::logic_error("invalid cards specification");
  }
  std::sort(result.begin(), result.end());
  if (std::adjacent_find(result.begin(), result.end()) != result.end()) {
    throw std::logic_error("duplicate card specified");
  }
  return Cards(result);
}

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

