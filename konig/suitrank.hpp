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

    explicit SuitRank(const std::string& s);

    bool valid() const { return value_ >= low_pip && value_ <= king; }

    operator internal_enum() const { return internal_enum(value_); }
    SuitRank& operator++() { ++value_; return *this; }
  private:
    uint8_t value_;
};

inline SuitRank::SuitRank(const std::string& s) {
  if (s.size() != 1) {
    throw std::logic_error("not a suit rank");
  }
  switch (s[0]) {
    case 'k':
    case 'K':
      value_ = king;
      break;
    case 'q':
    case 'Q':
      value_ = queen;
      break;
    case 'n':
    case 'N':
      value_ = knight;
      break;
    case 'j':
    case 'J':
      value_ = jack;
      break;
    case 't':
    case 'T':
    case 'a':
    case 'A':
    case '1':
      value_ = ten;
      break;
    case '2':
    case '9':
      value_ = nine;
      break;
    case '3':
    case '8':
      value_ = eight;
      break;
    case '4':
    case '7':
      value_ = seven;
      break;
    default:
      throw std::logic_error("not a suit rank");
  }
}

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

