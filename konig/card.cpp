#include <konig/card.hpp>

#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/include/classic_core.hpp>

namespace konig {

bool Card::from_string(Card& card, std::string const& description)
{
  using namespace boost::spirit::classic;
  Suit temp;
  return parse(
      description.c_str(),
      uint_p[
        px::ref(card) = px::construct<Card>(px::construct<TrumpRank>(arg1))
      ] |
      str_p("Sk")[px::ref(card) = Card(TrumpRank::skus)] |
      (
        (ch_p('C') | ch_p('D') | ch_p('H') | ch_p('S'))[
            px::ref(temp) =
              px::construct<Suit>(px::construct<std::string>(arg1, arg2))
          ] >>
        (
          ch_p('K') | ch_p('Q') | ch_p('N') | ch_p('J') |
          ch_p('t') | ch_p('9') | ch_p('8') | ch_p('7') |
          ch_p('1') | ch_p('2') | ch_p('3') | ch_p('4')
        )[
            px::ref(card) =
              px::construct<Card>(
                px::ref(temp),
                px::construct<SuitRank>(
                  px::construct<std::string>(arg1, arg2)
                )
              )
          ]
      ),
      space_p
    ).full;
}

Card::Card(const std::string& description)
{
  if (!from_string(*this, description)) {
    throw std::logic_error("invalid card specification");
  }
}

}

