#ifndef KONIG__SUIT_HPP
#define KONIG__SUIT_HPP

#include <stdexcept>

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

}

#endif // KONIG__SUIT_HPP

