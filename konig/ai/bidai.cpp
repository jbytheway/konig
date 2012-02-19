#include <konig/ai/bidai.hpp>

#include <konig/ai/nosuchai.hpp>
#include <konig/ai/linearmodelbidai.hpp>
#include <konig/ai/specificbidai.hpp>

namespace konig { namespace ai {

BidAi::Ptr BidAi::create(std::string const& description)
{
  std::string::const_iterator equals =
    std::find(description.begin(), description.end(), '=');
  std::string ai_name(description.begin(), equals);
  std::string ai_args(equals, description.end());
  if (!ai_args.empty()) {
    ai_args.erase(ai_args.begin());
  }
  BidAi::Ptr p;
  if (ai_name == "") {
    p.reset(new SpecificBidAi("contract=pass"));
  } else if (ai_name == "specific") {
    p.reset(new SpecificBidAi(ai_args));
  } else if (ai_name == "linearmodel") {
    p.reset(new LinearModelBidAi(ai_args));
  } else {
    throw NoSuchAi("BidAi: "+description);
  }
  return p;
}

BidAi::~BidAi() = default;

}}

