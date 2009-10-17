#ifndef KONIG__TABLEPOSITION_HPP
#define KONIG__TABLEPOSITION_HPP

#include <cstdint>
#include <cassert>
#include <istream>
#include <ostream>

namespace konig {

class TablePosition {
  public:
    typedef std::uint8_t internal_type;
    static const internal_type max = 5;

    // For the benefit of lexical_cast
    TablePosition() : val_(0) {}
    TablePosition(internal_type const v) : val_(v)
    {
      assert(val_ <= max);
    }
    operator internal_type() const { return val_; }
  private:
    internal_type val_;
};

inline std::ostream& operator<<(std::ostream& o, TablePosition const p) {
  o << int(p);
  return o;
}

inline std::istream& operator>>(std::istream& i, TablePosition& p) {
  int v;
  i >> v;
  p = v;
  return i;
}

}

#endif // KONIG__TABLEPOSITION_HPP

