#ifndef KONIG__BID_HPP
#define KONIG__BID_HPP

#include <cstdint>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/core.hpp>

namespace konig {

class Bid {
  friend class boost::serialization::access;
  public:
    typedef std::int32_t value_type;

    static Bid pass;

    Bid() = default;
    explicit Bid(value_type val);

    bool is_pass() const { return value_ == -1; }
    value_type value() const { return value_; }

    friend inline bool operator>=(Bid const& l, Bid const& r) {
      return l.value_ >= r.value_;
    }

    friend inline bool operator>(Bid const& l, Bid const& r) {
      return l.value_ > r.value_;
    }

    friend inline bool operator<(Bid const& l, Bid const& r) {
      return l.value_ < r.value_;
    }
  private:
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(value_);
    }

    value_type value_;
};

}

#endif // KONIG__BID_HPP

