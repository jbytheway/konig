#include <konig/cards.hpp>

#include <algorithm>

#include <boost/bind.hpp>
#include <boost/range/empty.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/stl/container.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace konig {

Cards Cards::make_deck()
{
  Cards result;
  Card::make_deck(std::inserter(result, result.begin()));
  return result;
}

Cards Cards::from_string(std::string const& description)
{
  Cards result;
  if (from_string(result, description)) {
    return result;
  } else {
    throw std::logic_error("invalid cards specification");
  }
}

namespace qi = boost::spirit::qi;

namespace {

struct CardChunk {
  Suit suit;
  std::vector<boost::optional<SuitRank>> ranks;
};

void insert_cards(std::vector<Card>& into, CardChunk from)
{
  BOOST_FOREACH(auto const& rank, from.ranks) {
    into.push_back(Card(from.suit, *rank));
  }
}

template<typename Iterator>
struct CardsGrammar :
  qi::grammar<Iterator, std::vector<Card>()>
{
  CardsGrammar() :
    CardsGrammar::base_type(start)
  {
    suit_parser =
      (qi::char_("CDHScdhs"))[
          qi::_val = px::bind(&Suit::from_char, qi::_1)
        ];

    suit_rank_parser = (
      qi::char_("KQNJTkqnjt9871234")
    )[ qi::_val = px::bind(&SuitRank::from_char, qi::_1) ];

    suit_ranks_parser %= *suit_rank_parser;

    trump_parser =
      qi::uint_[
        qi::_val = px::construct<Card>(px::construct<TrumpRank>(qi::_1))
      ] |
      qi::lit("Sk")[qi::_val = Card(TrumpRank::skus)];

    chunk_parser = (
      suit_parser[px::bind(&CardChunk::suit, qi::_val) = qi::_1] >>
      qi::lit(':') >>
      suit_ranks_parser[px::bind(&CardChunk::ranks, qi::_val) = qi::_1]
    ) | (
      suit_parser[px::bind(&CardChunk::suit, qi::_val) = qi::_1] >>
      suit_rank_parser[
        px::push_back(px::bind(&CardChunk::ranks, qi::_val), qi::_1)
      ]
    );

    start = *(
      (
        trump_parser[px::push_back(qi::_val, qi::_1)] |
        chunk_parser[px::bind(&insert_cards, qi::_val, qi::_1)]
      ) >> qi::omit[*(boost::spirit::ascii::space | qi::lit('_'))]
    );
  }

  qi::rule<Iterator, Suit()> suit_parser;
  qi::rule<Iterator, boost::optional<SuitRank>()> suit_rank_parser;
  qi::rule<Iterator, std::vector<boost::optional<SuitRank>>()>
    suit_ranks_parser;
  qi::rule<Iterator, Card()> trump_parser;
  qi::rule<Iterator, CardChunk()> chunk_parser;
  qi::rule<Iterator, std::vector<Card>()> start;
};

}

bool Cards::from_string(Cards& cards, std::string const& description)
{
  auto first = description.begin();
  auto const last = description.end();
  Suit temp;
  std::vector<Card> result;
  CardsGrammar<decltype(first)> cards_parser;

  bool x = qi::parse(
      first,
      last,
      cards_parser,
      result
    );
  if (!x || first != last) {
    return false;
  }
  std::sort(result.begin(), result.end());
  if (std::adjacent_find(result.begin(), result.end()) != result.end()) {
    std::ostringstream os;
    std::ostream_iterator<Card> oi(os, " ");
    std::copy(result.begin(), result.end(), oi);
    return false;
  }
  cards = Cards(result);
  return true;
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

