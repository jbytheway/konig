#include <konig/contracts.hpp>

#include <konig/contract.hpp>

namespace konig {

Bid Contracts::index_by_name(const std::string& name) const
{
  for (size_t i=0; i<contracts_.size(); ++i) {
    if (contracts_[i]->name() == name) {
      return Bid(i);
    }
  }
  return Bid::pass;
}

Bid Contracts::index_by_short_name(const std::string& short_name) const
{
  for (size_t i=0; i<contracts_.size(); ++i) {
    if (contracts_[i]->short_name() == short_name) {
      return Bid(i);
    }
  }
  return Bid::pass;
}

}

