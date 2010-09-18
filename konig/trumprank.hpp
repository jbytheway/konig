#ifndef KONIG__TRUMPRANK_HPP
#define KONIG__TRUMPRANK_HPP

#include <konig/fatal.hpp>
#include <konig/core.hpp>

namespace konig {

class TrumpRank {
  public:
    enum internal_enum {
      pagat = 1,
      uhu = 2,
      kakadu = 3,
      maribu = 4,
      mond = 21,
      skus = 22,
      max = 23
    };

    TrumpRank(const internal_enum v) : value_(v) {}
    explicit TrumpRank(const unsigned int v) : value_(v) {
      if (v < pagat || v > skus) {
        KONIG_FATAL("invalid trump rank " << v);
      }
    }

    bool valid() const { return value_ >= pagat && value_ <= skus; }

    operator internal_enum() const { return internal_enum(value_); }
    TrumpRank& operator++() { ++value_; return *this; }
    TrumpRank& operator--() { --value_; return *this; }
  private:
    uint8_t value_;
};

inline std::ostream& operator<<(std::ostream& o, const TrumpRank r) {
  if (r == TrumpRank::skus) {
    o << "Sk";
  } else {
    o << int(r);
  }
  return o;
}

}

#endif // KONIG__TRUMPRANK_HPP

