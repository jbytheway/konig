#include <konig/suitrank.hpp>

#include <stdexcept>
#include <ostream>

namespace konig {

SuitRank::SuitRank(const std::string& s) {
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

char SuitRank::to_char(bool is_red) const
{
  switch (value_) {
    case SuitRank::seven:
      if (is_red) {
        return '4';
      } else {
        return '7';
      }
    case SuitRank::eight:
      if (is_red) {
        return '3';
      } else {
        return '8';
      }
    case SuitRank::nine:
      if (is_red) {
        return '2';
      } else {
        return '9';
      }
    case SuitRank::ten:
      if (is_red) {
        return '1';
      } else {
        return 't';
      }
    case SuitRank::jack:
      return 'J';
    case SuitRank::knight:
      return 'N';
    case SuitRank::queen:
      return 'Q';
    case SuitRank::king:
      return 'K';
    default:
      throw std::logic_error("invalid suit rank");
  }
}

std::ostream& operator<<(std::ostream& o, SuitRank const r) {
  o << r.to_char();
  return o;
}

}

