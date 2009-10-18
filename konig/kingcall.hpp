#ifndef KONIG__KINGCALL_HPP
#define KONIG__KINGCALL_HPP

namespace konig {

class KingCall {
  friend class boost::serialization::access;
  public:
    enum internal_enum {
      clubs,
      diamonds,
      hearts,
      spades,
      fourth_king,
      invalid
    };

    KingCall() = default; // For serialization
    KingCall(internal_enum v) : value_(v) {}

    operator internal_enum() const { return value_; }
  private:
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(value_);
    }

    internal_enum value_;
};

}

#endif // KONIG__KINGCALL_HPP

