#include <konig/card.hpp>

#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/include/qi.hpp>

namespace konig {

bool Card::from_string(Card& card, std::string const& description)
{
  namespace qi = boost::spirit::qi;
  auto first = description.begin();
  auto const last = description.end();
  Suit temp;
  Card result;
  bool x = qi::parse(
    first,
    last,
    *qi::ascii::space >> (
      qi::uint_[
        px::ref(result) = px::construct<Card>(px::construct<TrumpRank>(qi::_1))
      ] |
      qi::lit("Sk")[px::ref(result) = Card(TrumpRank::skus)] |
      (
        (qi::char_("CDHScdhs"))[
            px::ref(temp) = px::bind(&Suit::from_char, qi::_1)
          ] >>
        (
          qi::char_("KQNJTkqnjt9871234")
        )[
            px::ref(result) =
              px::construct<Card>(
                px::ref(temp),
                px::bind(&SuitRank::from_char, qi::_1)
              )
          ]
      )
    ) >> *qi::ascii::space
  );
  if (!x || first != last) {
    return false;
  }
  card = result;
  return true;
}

Card::Card(const std::string& description)
{
  if (!from_string(*this, description)) {
    throw std::logic_error("invalid card specification");
  }
}

unsigned int Card::card_points() const {
  if (trump()) {
    auto rank = trump_rank();
    if (rank == TrumpRank::pagat ||
        rank == TrumpRank::mond ||
        rank == TrumpRank::skus)
      return 13;
    return 1;
  } else {
    switch (suit_rank()) {
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

std::ostream& operator<<(std::ostream& o, const Card& c) {
  if (c.trump()) {
    o << c.trump_rank();
  } else {
    o << c.suit() << c.suit_rank().to_char(c.suit().is_red());
  }
  return o;
}

}

