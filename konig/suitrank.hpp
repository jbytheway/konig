#ifndef KONIG__SUITRANK_HPP
#define KONIG__SUITRANK_HPP

#include <konig/core.hpp>

namespace konig {

class SuitRank {
  public:
    enum internal_enum {
      seven = 7,
      four = seven,
      low_pip = seven,
      eight = 8,
      three = eight,
      nine = 9,
      two = nine,
      ten = 10,
      one = ten,
      ace = one,
      jack = 11,
      knight = 12,
      queen = 13,
      king = 14
    };

    SuitRank(const internal_enum v) : value_(v) {}

    bool valid() const { return value_ >= 7 && value_ <= king; }

    operator internal_enum() const { return internal_enum(value_); }
    SuitRank& operator++() { ++value_; return *this; }
  private:
    uint8_t value_;
};

}

#endif // KONIG__SUITRANK_HPP

