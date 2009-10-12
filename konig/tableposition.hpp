#ifndef KONIG__TABLEPOSITION_HPP
#define KONIG__TABLEPOSITION_HPP

namespace konig {

class TablePosition {
  public:
    TablePosition(std::uint8_t const v) : val_(v)
    {
      assert(val_ <= 4);
    }
    operator std::uint8_t() { return val_; }
  private:
    std::uint8_t val_;
};

}

#endif // KONIG__TABLEPOSITION_HPP

