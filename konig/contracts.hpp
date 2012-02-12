#ifndef KONIG_CONTRACTS_HPP
#define KONIG_CONTRACTS_HPP

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

class KONIG_API Contracts {
  friend class boost::serialization::access;
  public:
    Contracts() : reserved_bids_(0) {}

    Contracts(boost::shared_ptr<Contract> c) :
      reserved_bids_(0)
    {
      contracts_.push_back(std::move(c));
    }

    Contracts(std::vector<boost::shared_ptr<Contract>> c, uint32_t r) :
      contracts_(std::move(c)),
      reserved_bids_(r)
    {
    }

    size_t size() const { return contracts_.size(); }
    Bid reserved_bids() const { return reserved_bids_; }
    Contract const& operator[](size_t const i) const {
      return *contracts_[i];
    }
    Contract const& operator[](Bid const i) const {
      return *contracts_[i.value()];
    }
    Contract const& at(size_t const i) const {
      return *contracts_.at(i);
    }
    Contract const& at(Bid const i) const {
      return *contracts_.at(i.value());
    }

    // Return Bid::pass when not found
    Bid index_by_name(const std::string& name) const;
    Bid index_by_bid_name(const std::string& short_name) const;
  private:
    template<typename Archive>
    void serialize(Archive& ar, unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(contracts_);
      ar & BOOST_SERIALIZATION_NVP(reserved_bids_);
    }

    // HACK: Would like to const Contracts here but it breaks
    // serialization
    //
    // Note that shared_ptr is warranted, because players store copies,
    // Outcomes store contracts and might outlive this object, and
    // this object is sent over the wire.
    std::vector<boost::shared_ptr<Contract>> contracts_;
    Bid reserved_bids_;
};

}

#endif // KONIG_CONTRACTS_HPP

