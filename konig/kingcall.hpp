#ifndef KONIG__KINGCALL_HPP
#define KONIG__KINGCALL_HPP

namespace konig {

class KingCall {
  public:
    enum internal_enum {
      clubs,
      diamonds,
      hearts,
      spades,
      fourth_king,
      invalid
    };

    KingCall(internal_enum v) : value_(v) {}

    operator internal_enum() const { return value_; }
  private:
    internal_enum value_;
};

}

#endif // KONIG__KINGCALL_HPP

