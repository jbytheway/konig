#ifndef KONIG_BID_HPP
#define KONIG_BID_HPP

#include <cstdint>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/api.hpp>
#include <konig/core.hpp>

namespace konig {

class KONIG_API Bid {
  friend class boost::serialization::access;
  public:
    typedef std::int32_t value_type;

    static Bid pass;

    Bid() = default;
    explicit Bid(value_type val);

    bool is_pass() const { return value_ == -1; }
    value_type value() const { return value_; }

    #define KONIG_BID_COMPARISON_OPERATOR(op) \
    friend inline bool operator op(Bid const& l, Bid const& r) { \
      return l.value_ op r.value_;            \
    }

    KONIG_BID_COMPARISON_OPERATOR(==)
    KONIG_BID_COMPARISON_OPERATOR(!=)
    KONIG_BID_COMPARISON_OPERATOR(>=)
    KONIG_BID_COMPARISON_OPERATOR(<=)
    KONIG_BID_COMPARISON_OPERATOR(>)
    KONIG_BID_COMPARISON_OPERATOR(<)

    #undef KONIG_BID_COMPARISON_OPERATOR
  private:
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(value_);
    }

    value_type value_;
};

}

#endif // KONIG_BID_HPP

