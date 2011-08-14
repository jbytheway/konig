#ifndef KONIG_SUIT_HPP
#define KONIG_SUIT_HPP

#include <stdexcept>
#include <ostream>
#include <cassert>

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
    bool is_red() const { return value_ == hearts || value_ == diamonds; }

    operator internal_enum() const { return internal_enum(value_); }
    Suit& operator++() { ++value_; return *this; }
  private:
    uint8_t value_;
};

std::ostream& operator<<(std::ostream& o, const Suit s);

}

#endif // KONIG_SUIT_HPP

