#include <konig/ai/specificbidai.hpp>

#include <boost/algorithm/string/join.hpp>

#include <optimal/optionsparser.hpp>

#include <konig/ai/aierror.hpp>

namespace konig { namespace ai {

SpecificBidAi::SpecificBidAi(std::string const& description)
{
  optimal::OptionsParser parser(':');
  parser.addOption("contract", '\0', &bid_name_);
  std::istringstream is(description);
  if (parser.parseStream(is, "bid=specific")) {
    throw AiError(boost::algorithm::join(parser.getErrors(), "\n"));
  }
}

void SpecificBidAi::reset(Ai const& ai)
{
  if (bid_name_ == "pass") {
    bid_ = Bid::pass;
  } else {
    Contracts const& contracts = ai.rules().contracts();
    bid_ = contracts.index_by_bid_name(bid_name_);
    if (bid_.is_pass()) {
      throw AiError("SpecificBidAi: no such bid '"+bid_name_+"'");
    }
  }
}

Bid SpecificBidAi::bid(Ai const& ai)
{
  Contracts const& contracts = ai.rules().contracts();
  if (ai.last_non_pass().is_pass() && bid_ < contracts.reserved_bids()) {
    return Bid(0);
  }
  if (bid_ >= ai.last_non_pass() &&
     (bid_ > ai.last_non_pass() || ai.position() == position_forehand)) {
    return bid_;
  }
  return Bid::pass;
}

}}

