#ifndef KONIG_KINGCALL_HPP
#define KONIG_KINGCALL_HPP

#include <string>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/api.hpp>
#include <konig/core.hpp>

namespace konig {

class KONIG_API KingCall {
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

    static KingCall from_value(const int v) {
      return KingCall(internal_enum(v));
    }
    static bool from_string(KingCall&, std::string const&);

    KingCall() = default; // For serialization
    KingCall(internal_enum v) : value_(v) {}

    std::string string() const;
    operator internal_enum() const { return value_; }
  private:
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(value_);
    }

    internal_enum value_;
};

std::ostream& operator<<(std::ostream&, KingCall const);

}

#endif // KONIG_KINGCALL_HPP

