#ifndef KONIG__TABLEPOSITION_HPP
#define KONIG__TABLEPOSITION_HPP

namespace konig {

class TablePosition {
  public:
    typedef std::uint8_t internal_type;
    static const internal_type max = 5;

    TablePosition(internal_type const v) : val_(v)
    {
      assert(val_ <= max);
    }
    operator internal_type() { return val_; }
  private:
    internal_type val_;
};

}

#endif // KONIG__TABLEPOSITION_HPP

