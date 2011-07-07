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

    static Suit from_value(const int v) { return Suit(internal_enum(v)); }
    static Suit from_char(char c) { return Suit(std::string(1, c)); }
    Suit() : value_(min) {}
    Suit(const internal_enum v) : value_(v) { assert(valid()); }
    explicit Suit(const int v) : value_(v) { assert(valid()); }

    explicit Suit(const std::string&);

    explicit Suit(const KingCall v) : value_(v) {
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

inline Suit::Suit(const std::string& s) {
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

