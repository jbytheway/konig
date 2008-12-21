#include <konig/deal.hpp>

namespace konig {

void Deal::sanity_check() {
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
  typedef std::ostream_iterator<Cards> out;
  d.copy_hands(out(o, "\n"));
  d.copy_talon(out(o, "\n"));
  return o;
}

}

