#include <konig/partialcards.hpp>

#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/stl/container.hpp>
#include <boost/spirit/core.hpp>

namespace konig {

PartialCards::PartialCards(const std::string& description, size_t size)
{
  using namespace boost::spirit;
  bool x = parse(
      description.c_str(),
      *(uint_p[px::push_back(px::ref(fixed_), px::construct<Card>(arg1))]),
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

