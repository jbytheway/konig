#include <konig/ai/ai.hpp>

#include <konig/ai/noddyai.hpp>
#include <konig/ai/specificplayai.hpp>

namespace konig { namespace ai {

Ai::Ptr Ai::create(const std::string& description)
{
  std::string::const_iterator colon =
    std::find(description.begin(), description.end(), ':');
  std::string ai_name(description.begin(), colon);
  std::string ai_args(colon, description.end());
  if (!ai_args.empty()) {
    ai_args.erase(ai_args.begin());
  }
  Ai::Ptr p;
  if (ai_name == "") {
    p.reset(new NoddyAi());
  } else if (ai_name == "play") {
    p.reset(new SpecificPlayAi(ai_args));
  } else {
    throw std::logic_error("no such ai");
  }
  return p;
}

Ai::Ai() :
  position_(PlayPosition(-1)),
  last_non_pass_(-1),
  declarer_(position_forehand),
  offence_(false),
  king_call_(KingCall::invalid)
{}

void Ai::start_game(Ruleset rules, PlayPosition pos, Cards hand)
{
  rules_ = std::move(rules);
  position_ = pos;
  hand_ = std::move(hand);
  bidding_.clear();
  last_non_pass_ = -1;
  declarer_ = position_forehand;
  offence_ = false;
  contract_ = ContractAndAnnouncements();
  king_call_ = KingCall::invalid;
  talon_[0].clear();
  talon_[1].clear();
  accepted_.clear();
  rejected_.clear();
  discard_.clear();
  tricks_.clear();
}

void Ai::notify_bid(PlayPosition p, int bid)
{
  if (bid < -1 || bid >= int(rules_.contracts().size())) {
    throw std::logic_error("invalid bid");
  }
  if (bid != -1) {
    declarer_ = p;
    last_non_pass_ = bid;
    offence_ = (p == position_);
    contract_ = ContractAndAnnouncements(rules_.contracts().at(last_non_pass_));
    PlayPosition first_leader = position_forehand;
    if (contract_.contract()->grants_lead()) {
      first_leader = declarer_;
    }
    tricks_.clear();
    tricks_.push_back(Trick(first_leader, contract_.contract()->rising_rule()));
  }
  bidding_.push_back(bid);
}

void Ai::notify_call_king(KingCall call)
{
  king_call_ = call;
  if (call == KingCall::fourth_king) {
    Cards::iterator king = hand_.find(SuitRank::king);
    if (king != hand_.end()) {
      offence_ = true;
      contract_.set_called_king(*king);
    }
  } else {
    Card called_king(call);
    offence_ = hand_.count(called_king);
    contract_.set_called_king(called_king);
  }
}

void Ai::notify_talon(const boost::array<Cards, 2>& talon)
{
  talon_ = talon;
  accepted_ = talon_[0];
  accepted_.insert(talon_[1]);
}

void Ai::notify_talon_choice(uint8_t choice)
{
  assert(choice < 2);
  accepted_ = talon_[choice];
  rejected_ = talon_[!choice];
}

void Ai::notify_discard(Cards discard)
{
  discard_ = std::move(discard);
}

void Ai::notify_announcements(std::vector<Announcement> announcements)
{
  contract_.add(std::move(announcements));
}

void Ai::notify_play_card(PlayPosition p, Card c)
{
  assert(!tricks_.empty());
  assert(tricks_.back().played() > 0 || tricks_.back().leader() == p);
  assert(!tricks_.back().complete());
  tricks_.back().add(c);
  if (tricks_.back().complete()) {
    assert(tricks_.size() < 13);
    if (tricks_.size() < 12) {
      tricks_.push_back(
          Trick(tricks_.back().winner(), contract_.contract()->rising_rule())
        );
    }
  }
  if (p == position_) {
    assert(hand_.count(c));
    hand_.erase(c);
  }
}

Cards Ai::legal_plays() const
{
  assert(!tricks_.empty());
  return tricks_.back().legal_plays(
      hand_, offence_, 13-tricks_.size(), contract_
    );
}

}}

