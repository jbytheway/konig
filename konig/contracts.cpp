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

Bid Contracts::index_by_bid_name(const std::string& bid_name) const
{
  for (size_t i=0; i<contracts_.size(); ++i) {
    if (contracts_[i]->bid_name() == bid_name) {
      return Bid(i);
    }
  }
  return Bid::pass;
}

}

