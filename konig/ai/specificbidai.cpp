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

Bid SpecificBidAi::bid(Ai const& ai)
{
  if (bid_name_ == "pass") return Bid::pass;
  Contracts const& contracts = ai.rules().contracts();
  Bid bid = contracts.index_by_bid_name(bid_name_);
  if (bid.is_pass()) {
    throw AiError("SpecificBidAi: no such bid '"+bid_name_+"'");
  }
  if (ai.last_non_pass().is_pass() && bid < contracts.reserved_bids()) {
    return Bid(0);
  }
  if (bid >= ai.last_non_pass() &&
     (bid > ai.last_non_pass() || ai.position() == position_forehand)) {
    return bid;
  }
  return Bid::pass;
}

}}

