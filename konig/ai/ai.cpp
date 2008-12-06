#include <konig/ai/ai.hpp>

#include <konig/ai/noddyai.hpp>

namespace konig { namespace ai {

Ai::Ptr Ai::create(const std::string& /*description*/)
{
  Ai::Ptr p(new NoddyAi());
  return p;
}

Ai::Ai() :
  position_(PlayPosition(-1))
{}

void Ai::start_game(const Ruleset& rules, PlayPosition pos, const Cards& hand)
{
  rules_ = rules;
  position_ = pos;
  hand_ = hand;
  bidding_.clear();
}

void Ai::notify_bid(int bid)
{
  if (bid < -1 || bid >= int(rules_.contracts().size())) {
    throw std::logic_error("invalid bid");
  }
  bidding_.push_back(bid);
}

}}

