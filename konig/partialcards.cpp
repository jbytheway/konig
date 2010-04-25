#include <konig/partialcards.hpp>

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

PartialCards::PartialCards(const std::string& description, size_t size)
{
  namespace qi = boost::spirit::qi;
  auto first = description.begin();
  auto const last = description.end();
  Suit temp;
  bool x = qi::phrase_parse(
      first,
      last,
      *(
        qi::uint_[px::push_back(
            px::ref(fixed_),
            px::construct<Card>(px::construct<TrumpRank>(qi::_1))
          )] |
        qi::lit("Sk")[px::push_back(px::ref(fixed_), Card(TrumpRank::skus))] |
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
                px::ref(fixed_),
                px::construct<Card>(
                  px::ref(temp),
                  px::bind(&SuitRank::from_char, qi::_1)
                )
              )
            ]
        )
      ),
      qi::space
    );
  if (!x || first != last) {
    throw std::logic_error("invalid partial cards specification");
  }
  std::sort(fixed_.begin(), fixed_.end());
  if (std::adjacent_find(fixed_.begin(), fixed_.end()) != fixed_.end()) {
    throw std::logic_error("duplicate card specified");
  }
  if (size < fixed_.size()) {
    throw std::logic_error("too many cards specified");
  }
  num_random_ = size - fixed_.size();
}

}

