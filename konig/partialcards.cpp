#include <konig/partialcards.hpp>

#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/stl/container.hpp>
#include <boost/spirit/core.hpp>

namespace konig {

PartialCards::PartialCards(const std::string& description, size_t size)
{
  using namespace boost::spirit;
  Suit temp;
  bool x = parse(
      description.c_str(),
      *(
        uint_p[px::push_back(px::ref(fixed_), px::construct<Card>(arg1))] |
        str_p("Sk")[px::push_back(px::ref(fixed_), Card(TrumpRank::skus))] |
        (
          (ch_p('C') | ch_p('D') | ch_p('H') | ch_p('S'))[
              px::ref(temp) =
                px::construct<Suit>(px::construct<std::string>(arg1, arg2))
            ] >>
          ch_p(':') >>
          *(
            ch_p('K') | ch_p('Q') | ch_p('N') | ch_p('J') |
            ch_p('t') | ch_p('9') | ch_p('8') | ch_p('7') |
            ch_p('1') | ch_p('2') | ch_p('3') | ch_p('4')
          )[
              px::push_back(
                px::ref(fixed_),
                px::construct<Card>(
                  px::ref(temp),
                  px::construct<SuitRank>(
                    px::construct<std::string>(arg1, arg2)
                  )
                )
              )
            ]
        )
      ),
      space_p
    ).full;
  if (!x) {
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

