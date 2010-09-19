#include <konig/ai/ai.hpp>

#include <boost/assign/list_of.hpp>

#include <konig/fatal.hpp>
#include <konig/ai/nosuchai.hpp>
#include <konig/ai/noddyai.hpp>
#include <konig/ai/specificplayai.hpp>
#include <konig/ai/forwardingai.hpp>
#include <konig/ai/invalidplayerror.hpp>

namespace konig { namespace ai {

Ai::Ptr Ai::create(const std::string& description)
{
  std::string::const_iterator equals =
    std::find(description.begin(), description.end(), '=');
  std::string ai_name(description.begin(), equals);
  std::string ai_args(equals, description.end());
  if (!ai_args.empty()) {
    ai_args.erase(ai_args.begin());
  }
  Ai::Ptr p;
  if (ai_name == "") {
    p.reset(new NoddyAi());
  } else if (ai_name == "play") {
    p.reset(new SpecificPlayAi(ai_args));
  } else if (ai_name == "forward") {
    p.reset(new ForwardingAi(ai_args));
  } else {
    throw NoSuchAi(ai_name);
  }
  return p;
}

Ai::Ai() :
  position_(PlayPosition(-1)),
  last_non_pass_(-1),
  declarer_(position_forehand),
  offence_(false),
  king_call_(KingCall::invalid),
  called_king_in_talon_(false)
{}

void Ai::start_game(Ruleset rules, PlayPosition pos, Cards hand)
{
  assert(hand.size() == 12);
  rules_ = std::move(rules);
  position_ = pos;
  hand_ = std::move(hand);
  bidding_.clear();
  last_non_pass_ = Bid::pass;
  declarer_ = position_forehand;
  offence_ = false;
  contract_ = ContractAndAnnouncements();
  king_call_ = KingCall::invalid;
  talon_[0].clear();
  talon_[1].clear();
  called_king_in_talon_ = false;
  accepted_.clear();
  rejected_.clear();
  discard_.clear();
  tricks_.clear();

  game_start_hook();
}

void Ai::notify_bid(PlayPosition p, Bid bid)
{
  if (bid < Bid::pass || bid >= Bid(rules_.contracts().size())) {
    KONIG_FATAL("invalid bid " << bid.value());
  }
  if (!bid.is_pass()) {
    declarer_ = p;
    last_non_pass_ = bid;
    offence_ = (p == position_);
    contract_ = ContractAndAnnouncements(rules_.contracts().at(last_non_pass_));
    PlayPosition first_leader = position_forehand;
    if (contract_.contract()->grants_lead()) {
      first_leader = declarer_;
    }
    tricks_.clear();
    tricks_.push_back(Trick(
        first_leader, contract_.contract()->rising_rule(),
        contract_.contract()->hold_pagat()
    ));
  }
  bidding_.push_back(bid);
}

void Ai::notify_contract_established(Bid)
{
  contract_established_hook();
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
    Card called_king(Suit(call), SuitRank::king);
    offence_ = offence_ || hand_.count(called_king);
    contract_.set_called_king(called_king);
  }
}

void Ai::notify_talon(const std::array<Cards, 2>& talon)
{
  talon_ = talon;
  accepted_ = talon_[0];
  accepted_.insert(talon_[1]);

  // Check for called king
  if (king_call_ == KingCall::fourth_king) {
    Cards::iterator king = accepted_.find(SuitRank::king);
    if (king != accepted_.end()) {
      called_king_in_talon_ = true;
      contract_.set_called_king(*king);
    }
  } else if (king_call_ != KingCall::invalid) {
    Card called_king(Suit(king_call_), SuitRank::king);
    called_king_in_talon_ = accepted_.count(called_king);
  }
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
  if (position() == declarer()) {
    hand_.insert(accepted_);
    hand_.erase(discard);
    assert(hand_.size() == 12);
  }
}

void Ai::notify_announcements(std::vector<Announcement> announcements)
{
  contract_.add(std::move(announcements));
  if (contract_.is_done()) {
    play_start_hook();
  }
}

void Ai::notify_play_card(PlayPosition p, Card c)
{
  assert(!tricks_.empty());
  assert(tricks_.back().played() > 0 || tricks_.back().leader() == p);
  assert(!tricks_.back().complete());
  tricks_.back().add(c);
  if (tricks_.back().complete()) {
    assert(tricks_.size() < 13);
    trick_complete_hook();
    if (tricks_.size() < 12) {
      tricks_.push_back(Trick(
          tricks_.back().winner(), contract_.contract()->rising_rule(),
          contract_.contract()->hold_pagat()
      ));
    }
  }
  if (p == position_) {
    assert(hand_.count(c));
    hand_.erase(c);
  }
}

void Ai::notify_invalid_play(std::string m)
{
  throw InvalidPlayError(std::move(m));
}

uint8_t Ai::trick_number() const
{
  return 13 - tricks().size();
}

uint8_t Ai::guess_num_offence() const
{
  // For non-partnership contracts it's always 1
  if (king_call_ == KingCall::invalid) return 1;
  // For partnership contracts we guess 2 unless we saw the called king in the
  // talon
  return called_king_in_talon_ ? 1 : 2;
}

Cards Ai::legal_plays() const
{
  assert(!tricks_.empty());
  return tricks_.back().legal_plays(
      hand_, offence_, trick_number(), contract_
    );
}

boost::optional<Card> Ai::relevant_bird() const
{
  unsigned int trick_num = trick_number();
  boost::optional<Card> result;
  // HACK: relying on Cheltenham rules
  assert(trick_num > 0);
  if (trick_num <= 3) {
    result = Card(TrumpRank(trick_num));
  }
  return result;
}

}}

