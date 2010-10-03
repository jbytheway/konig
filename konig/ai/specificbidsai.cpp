#include <konig/ai/specificbidsai.hpp>

namespace konig { namespace ai {

SpecificBidsAi::SpecificBidsAi(std::vector<Bid> bids) :
  bids_(std::move(bids)),
  next_bid_(bids_.begin())
{}

Bid SpecificBidsAi::bid(Ai const&) {
  if (next_bid_ == bids_.end()) return Bid::pass;
  Bid b = *next_bid_;
  ++next_bid_;
  return b;
}

}}

