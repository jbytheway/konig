#ifndef KONIG_SUITRANK_HPP
#define KONIG_SUITRANK_HPP

#include <cstdint>
#include <cassert>
#include <string>

#include <konig/core.hpp>

namespace konig {

class SuitRank {
  public:
    enum internal_enum {
      seven = 7,
      four = seven,
      low_pip = seven,
      min = seven,
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
      king = 14,
      max = 15
    };

    static SuitRank from_value(const std::uint8_t v) {
      return SuitRank(internal_enum(v));
    }

    static SuitRank from_char(const char c) {
      return SuitRank(std::string(1, c));
    }

    SuitRank(const internal_enum v) : value_(v) { assert(valid()); }

    explicit SuitRank(const std::string& s);

    bool valid() const { return value_ >= low_pip && value_ <= king; }
    bool face() const { assert(valid()); return value_ >= jack; }

    char to_char(bool is_red = false) const;

    operator internal_enum() const { return internal_enum(value_); }
    SuitRank& operator++() { ++value_; return *this; }
    SuitRank& operator--() { --value_; return *this; }
  private:
    std::uint8_t value_;
};

std::ostream& operator<<(std::ostream& o, const SuitRank r);

}

#endif // KONIG_SUITRANK_HPP

