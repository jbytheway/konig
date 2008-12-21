#include <konig/contracts.hpp>

#include <konig/contract.hpp>

namespace konig {

int Contracts::index_of_contract(const std::string& name) const
{
  for (size_t i=0; i<contracts_.size(); ++i) {
    if (contracts_[i]->name() == name) {
      return i;
    }
  }
  throw std::logic_error("no such contract '"+name+"'");
}

}

