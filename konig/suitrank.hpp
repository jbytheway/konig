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

inline std::ostream& operator<<(std::ostream& o, const SuitRank r) {
  char name;
  switch (r) {
    case SuitRank::seven:
      name = '7';
      break;
    case SuitRank::eight:
      name = '8';
      break;
    case SuitRank::nine:
      name = '9';
      break;
    case SuitRank::ten:
      name = 't';
      break;
    case SuitRank::jack:
      name = 'J';
      break;
    case SuitRank::knight:
      name = 'N';
      break;
    case SuitRank::queen:
      name = 'Q';
      break;
    case SuitRank::king:
      name = 'K';
      break;
    default:
      throw std::logic_error("invalid suit rank");
  }
  return o << name;
}

}

#endif // KONIG__SUITRANK_HPP

