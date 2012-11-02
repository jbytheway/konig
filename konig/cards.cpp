#include <konig/cards.hpp>

#include <algorithm>

#include <boost/bind.hpp>
#include <boost/range/empty.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/adaptor/reversed.hpp>
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
      -qi::lit(':') >>
      suit_ranks_parser[px::bind(&CardChunk::ranks, qi::_val) = qi::_1]
    );

    start = qi::omit[*(boost::spirit::ascii::space | qi::lit('_'))] >>
      *(
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

namespace {

  struct PointsHelper {
    static constexpr CardPoints max_card_points = 4;
    PointsHelper() {
      for (Card::index_type i = 0; i != Card::index_max; ++i) {
        auto c = Card::from_index(i);
        auto p = c.card_points();
        assert(p >= 1);
        p -= 1;
        assert(p % 3 == 0);
        p /= 3;
        assert(p <= max_card_points);
        for (CardPoints q = 0; q < p; ++q) {
          cards_worth_at_least[q].insert(c);
        }
      }
    }
    std::array<Cards, max_card_points> cards_worth_at_least;
  };

  PointsHelper points_helper;

}

CardPoints Cards::total_card_points() const {
  CardPoints tally = 0;
  for (CardPoints p = 0; p < PointsHelper::max_card_points; ++p) {
    Cards tmp(*this);
    intersection_into(tmp, points_helper.cards_worth_at_least[p]);
    tally += tmp.size();
  }
  return tally*3+size();
}

void Cards::dump(std::ostream& o, bool machine_readable) const
{
  if (empty()) {
    if (machine_readable) {
      o << '_';
    }
    return;
  }

  char const* const sep = machine_readable ? "_" : " ";
  typedef std::pair<Cards::const_iterator, Cards::const_iterator> Range;
  auto const trumps = equal_range(Suit::trumps);

  boost::range::transform(
      boost::adaptors::reverse(trumps),
      std::ostream_iterator<TrumpRank>(o, sep),
      [](Card const& c) { return c.trump_rank(); }
    );

  for (Suit s = Suit::min; s<Suit::trumps; ++s) {
    auto const suit = equal_range(s);
    if (!boost::empty(suit)) {
      o << s << ':';
      boost::range::transform(
          boost::adaptors::reverse(suit), std::ostream_iterator<char>(o),
          [s](Card const& c) { return c.suit_rank().to_char(s.is_red()); }
        );
      o << sep;
    }
  }
}

std::ostream& operator<<(std::ostream& o, const Cards& c)
{
  c.dump(o);
  return o;
}

}

