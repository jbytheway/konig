#include <konig/ai/forwardingai.hpp>

#include <boost/algorithm/string/join.hpp>

#include <optionsparser.h>

#include <konig/fatal.hpp>
#include <konig/ai/aierror.hpp>

namespace konig { namespace ai {

ForwardingAi::ForwardingAi(std::string const& args)
{
  std::string bid_spec;
  std::string play_spec;
  optimal::OptionsParser parser(' ');
  parser.addOption("bid", '\0', &bid_spec);
  parser.addOption("play", '\0', &play_spec);
  std::istringstream is(args);
  if (parser.parseStream(is, "forward")) {
    throw AiError(boost::algorithm::join(parser.getErrors(), "\n"));
  }
  bidder_ = BidAi::create(bid_spec);
  player_ = PlayAi::create(play_spec);
}

Bid ForwardingAi::bid()
{
  return bidder_->bid(*this);
}

KingCall ForwardingAi::call_king()
{
  KONIG_FATAL("not implemented");
}

uint8_t ForwardingAi::choose_talon_half()
{
  KONIG_FATAL("not implemented");
}

Cards ForwardingAi::discard()
{
  KONIG_FATAL("not implemented");
}

std::vector<Announcement> ForwardingAi::announce()
{
  KONIG_FATAL("not implemented");
}

Card ForwardingAi::play_card()
{
  return player_->play_card(*this);
}

}}

