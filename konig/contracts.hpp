#ifndef KONIG__CONTRACTS_HPP
#define KONIG__CONTRACTS_HPP

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <konig/bid.hpp>
#include <konig/contract.hpp>

namespace konig {

class Contract;

class Contracts {
  friend class boost::serialization::access;
  public:
    Contracts() : reserved_contracts_(0) {}

    Contracts(boost::shared_ptr<Contract> c) :
      reserved_contracts_(0)
    {
      contracts_.push_back(c);
    }

    size_t size() const { return contracts_.size(); }
    const boost::shared_ptr<Contract const> operator[](size_t const i) const {
      return contracts_[i];
    }
    const boost::shared_ptr<Contract const> operator[](Bid const i) const {
      return contracts_[i.value()];
    }
    const boost::shared_ptr<Contract const> at(size_t const i) const {
      return contracts_.at(i);
    }
    const boost::shared_ptr<Contract const> at(Bid const i) const {
      return contracts_.at(i.value());
    }

    // Return Bid::pass when not found
    Bid index_by_name(const std::string& name) const;
    Bid index_by_short_name(const std::string& short_name) const;
  private:
    template<typename Archive>
    void serialize(Archive& ar, unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(contracts_);
      ar & BOOST_SERIALIZATION_NVP(reserved_contracts_);
    }

    // HACK: Would like to use const Contracts here but it breaks serialization
    std::vector<boost::shared_ptr<Contract>> contracts_;
    size_t reserved_contracts_;
};

}

#endif // KONIG__CONTRACTS_HPP

