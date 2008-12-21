#ifndef KONIG__SUIT_HPP
#define KONIG__SUIT_HPP

#include <stdexcept>
#include <ostream>

#include <konig/core.hpp>
#include <konig/kingcall.hpp>

namespace konig {

class Suit {
  public:
    enum internal_enum {
      club,
      clubs = club,
      min = club,
      diamond,
      diamonds = diamond,
      heart,
      hearts = heart,
      spade,
      spades = spade,
      trump,
      trumps = trump,
      max
    };

    Suit(const internal_enum v) : value_(v) {}
    Suit(const KingCall v) : value_(v) {
      if (v == KingCall::fourth_king) {
        throw std::logic_error("not a suit");
      }
    }

    bool valid() const { return /*value_ >= min &&*/ value_ < max; }

    operator internal_enum() const { return internal_enum(value_); }
    Suit& operator++() { ++value_; return *this; }
  private:
    uint8_t value_;
};

inline std::ostream& operator<<(std::ostream& o, const Suit s) {
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

#endif // KONIG__SUIT_HPP

