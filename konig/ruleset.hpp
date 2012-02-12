#ifndef KONIG_RULESET_HPP
#define KONIG_RULESET_HPP

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/contracts.hpp>

namespace konig {

class KONIG_API Ruleset {
  friend class boost::serialization::access;
  public:
    Ruleset() {}
    explicit Ruleset(Contracts contracts) :
      contracts_(std::move(contracts))
    {}

    const Contracts& contracts() const { return contracts_; }

    static Ruleset cheltenham();
    static Ruleset solodreier_only();
  private:
    template<typename Archive>
    void serialize(Archive& ar, unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(contracts_);
    }

    Contracts contracts_;
};

}

#endif // KONIG_RULESET_HPP

