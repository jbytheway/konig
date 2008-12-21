#include <konig/ai/ai.hpp>

#include <konig/ai/noddyai.hpp>

namespace konig { namespace ai {

Ai::Ptr Ai::create(const std::string& /*description*/)
{
  Ai::Ptr p(new NoddyAi());
  return p;
}

Ai::Ai() :
  position_(PlayPosition(-1)),
  last_non_pass_(-1),
  king_call_(KingCall::invalid)
{}

void Ai::start_game(Ruleset rules, PlayPosition pos, Cards hand)
{
  rules_ = std::move(rules);
  position_ = pos;
  hand_ = std::move(hand);
  bidding_.clear();
}

void Ai::notify_bid(int bid)
{
  if (bid < -1 || bid >= int(rules_.contracts().size())) {
    throw std::logic_error("invalid bid");
  }
  if (bid != -1)
    last_non_pass_ = bid;
  bidding_.push_back(bid);
}

void Ai::notify_call_king(KingCall call)
{
  king_call_ = call;
}

void Ai::notify_talon(const boost::array<Cards, 2>& talon)
{
  talon_ = talon;
  accepted = talon_[0];
  accepted.insert(talon_[1]);
}

void Ai::notify_talon_choice(uint8_t choice)
{
  assert(choice < 2);
  accepted = talon_[choice];
  rejected = talon_[!choice];
}

void Ai::notify_discard(Cards discard)
{
  discard_ = std::move(discard);
}

void Ai::notify_announcements(std::vector<Announcement> announcements)
{
  if (!contract_) {
    contract_ = rules_.contracts().at(last_non_pass_);
  }
  announcements_.push_back(std::move(announcements));
}

void Ai::notify_play_card(PlayPosition p, Card c)
{
  if (tricks_.empty() || tricks_.back().complete()) {
    assert(tricks_.size() < 13);
    tricks_.push_back(Trick(p, contract_->rising_rule()));
  }
  tricks_.back().add(c);
  if (p == position_) {
    assert(hand_.count(c));
    hand_.erase(c);
  }
}

}}

