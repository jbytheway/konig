#ifndef KONIG__CONTRACTS_HPP
#define KONIG__CONTRACTS_HPP

#include <vector>

#include <boost/shared_ptr.hpp>

#include <konig/core.hpp>

namespace konig {

class Contract;

class Contracts {
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
    std::vector<boost::shared_ptr<Contract>> contracts_;
    size_t reserved_contracts_;
};

}

#endif // KONIG__CONTRACTS_HPP

