#ifndef KONIG__CONTRACTS_HPP
#define KONIG__CONTRACTS_HPP

#include <vector>

#include <konig/contract.hpp>

namespace konig {

class Contracts {
  public:
    Contracts() : reserved_contracts_(0) {}

    Contracts(Contract::Ptr c) :
      reserved_contracts_(0)
    {
      contracts_.push_back(c);
    }

    size_t size() const { return contracts_.size(); }
    const Contract::Ptr& operator[](size_t i) const { return contracts_[i]; }
  private:
    std::vector<Contract::Ptr> contracts_;
    size_t reserved_contracts_;
};

}

#endif // KONIG__CONTRACTS_HPP

