#include <konig/ai/bidai.hpp>

#include <konig/ai/nosuchai.hpp>

namespace konig { namespace ai {

BidAi::Ptr BidAi::create(std::string const& description)
{
  throw NoSuchAi("BidAi: "+description);
}

BidAi::~BidAi() = default;

}}

