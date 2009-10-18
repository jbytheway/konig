#ifndef KONIG__CONTRACTS_HPP
#define KONIG__CONTRACTS_HPP

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

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
    const boost::shared_ptr<Contract>& operator[](size_t i) const {
      return contracts_[i];
    }
    const boost::shared_ptr<Contract>& at(size_t i) const {
      return contracts_.at(i);
    }

    int index_of_contract(const std::string& name) const;
  private:
    template<typename Archive>
    void serialize(Archive& ar, unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(contracts_);
      ar & BOOST_SERIALIZATION_NVP(reserved_contracts_);
    }

    std::vector<boost::shared_ptr<Contract>> contracts_;
    size_t reserved_contracts_;
};

}

#endif // KONIG__CONTRACTS_HPP

