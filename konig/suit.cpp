#include <konig/suit.hpp>

namespace konig {

Suit::Suit(const std::string& s) {
  if (s.size() != 1) {
    throw std::logic_error("not a suit");
  }
  switch (s[0]) {
    case 'c':
    case 'C':
      value_ = clubs;
      break;
    case 'd':
    case 'D':
      value_ = diamonds;
      break;
    case 'h':
    case 'H':
      value_ = hearts;
      break;
    case 's':
    case 'S':
      value_ = spades;
      break;
    case 't':
    case 'T':
      value_ = trumps;
      break;
    default:
      throw std::logic_error("not a suit");
  }
}

std::ostream& operator<<(std::ostream& o, const Suit s) {
  char name;
  switch (s) {
    case Suit::clubs:
      name = 'C';
      break;
    case Suit::diamonds:
      name = 'D';
      break;
    case Suit::hearts:
      name = 'H';
      break;
    case Suit::spades:
      name = 'S';
      break;
    case Suit::trumps:
      name = 'T';
      break;
    default:
      throw std::logic_error("invalid suit");
  }
  return o << name;
}

}

