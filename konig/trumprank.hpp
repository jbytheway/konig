#ifndef KONIG__TRUMPRANK_HPP
#define KONIG__TRUMPRANK_HPP

#include <stdexcept>

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
      skus = 22
    };

    TrumpRank(const internal_enum v) : value_(v) {}
    explicit TrumpRank(const unsigned int v) : value_(v) {
      if (v < pagat || v > skus) {
        throw std::logic_error("invalid trump rank");
      }
    }

    bool valid() const { return value_ >= pagat && value_ <= skus; }

    operator internal_enum() const { return internal_enum(value_); }
    TrumpRank& operator++() { ++value_; return *this; }
  private:
    uint8_t value_;
};

}

#endif // KONIG__TRUMPRANK_HPP

