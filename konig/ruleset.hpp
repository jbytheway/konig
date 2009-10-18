#ifndef KONIG__RULESET_HPP
#define KONIG__RULESET_HPP

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/contracts.hpp>

namespace konig {

class Ruleset {
  friend class boost::serialization::access;
  public:
    Ruleset() {}
    explicit Ruleset(const Contracts& contracts) :
      contracts_(contracts)
    {}

    const Contracts& contracts() const { return contracts_; }

    static Ruleset solodreier_only();
  private:
    template<typename Archive>
    void serialize(Archive& ar, unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(contracts_);
    }

    Contracts contracts_;
};

}

#endif // KONIG__RULESET_HPP

