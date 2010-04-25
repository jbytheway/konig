#include <konig/deal.hpp>

namespace konig {

void Deal::write(std::ostream& o, std::string delimiter) const
{
  typedef std::ostream_iterator<Cards> out;
  copy_hands(out(o, delimiter.c_str()));
  copy_talon(out(o, delimiter.c_str()));
}

void Deal::sanity_check() const
{
  BOOST_FOREACH(const Cards& hand, hands_) {
    if (hand.size() != 12) {
      throw std::logic_error("wrong number cards in hand");
    }
  }

  BOOST_FOREACH(const Cards& talon_half, talon_) {
    if (talon_half.size() != 3) {
      throw std::logic_error("wrong number cards in talon-half");
    }
  }
}

std::ostream& operator<<(std::ostream& o, const Deal& d)
{
  d.write(o, "\n");
  return o;
}

}

