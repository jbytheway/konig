#include <konig/ai/forwardingai.hpp>

#include <iostream>

#include <boost/algorithm/string/join.hpp>

#include <optimal/optionsparser.hpp>

#include <konig/fatal.hpp>
#include <konig/ai/aierror.hpp>
#include <konig/ai/roffenceai.hpp>
#include <konig/ai/rpartnerai.hpp>
#include <konig/ai/rdefenceai.hpp>
#include <konig/ai/sedoffenceai.hpp>
#include <konig/ai/seddefenceai.hpp>
#include <konig/ai/tai.hpp>
#include <konig/ai/poffenceai.hpp>
#include <konig/ai/pdefenceai.hpp>
#include <konig/ai/boffenceai.hpp>
#include <konig/ai/bdefenceai.hpp>
#include <konig/ai/sdoffenceai.hpp>
#include <konig/ai/sddefenceai.hpp>

namespace konig { namespace ai {

ForwardingAi::ForwardingAi(std::string const& args) :
  debug_(false),
  auto_play_(false)
{
  std::string bid_spec;
  std::string announce_spec;
  std::string play_spec;
  optimal::OptionsParser parser(' ');
  parser.addOption("bid", '\0', &bid_spec);
  parser.addOption("announce", '\0', &announce_spec);
  parser.addOption("play", '\0', &play_spec);
  parser.addOption("debug", '\0', &debug_);
  std::istringstream is(args);
  if (parser.parseStream(is, "forward")) {
    throw AiError(boost::algorithm::join(parser.getErrors(), "\n"));
  }
  bidder_ = BidAi::create(bid_spec);
  announcer_ = AnnouncementAi::create(announce_spec);
  if (play_spec == "auto") {
    auto_play_ = true;
  } else {
    player_ = PlayAi::create(play_spec);
  }
}

ForwardingAi::ForwardingAi(
  BidAi::Ptr bidder,
  AnnouncementAi::Ptr announcer,
  PlayAi::Ptr player
) :
  debug_(false),
  auto_play_(false),
  bidder_(std::move(bidder)),
  announcer_(std::move(announcer)),
  player_(std::move(player))
{
}

void ForwardingAi::notify_call_king(KingCall call)
{
  FateAi::notify_call_king(call);

  // Now the partnerships are known, so non-declarers can have their
  // AnnouncementAis reset
  if (position() != declarer()) {
    announcer_->reset(*this);
  }
}

Bid ForwardingAi::bid()
{
  return bidder_->bid(*this);
}

KingCall ForwardingAi::call_king()
{
  return announcer_->call_king(*this);
}

uint8_t ForwardingAi::choose_talon_half()
{
  return announcer_->choose_talon_half(*this);
}

Cards ForwardingAi::discard()
{
  return announcer_->discard(*this);
}

std::vector<Announcement> ForwardingAi::announce()
{
  return announcer_->announce(*this);
}

Card ForwardingAi::play_card()
{
  return player_->play_card(*this);
}

void ForwardingAi::contract_established_hook()
{
  FateAi::contract_established_hook();

  // Prepare announcer for declarer, and (in non-partnership contracts) for
  // everyone else
  if (position() == declarer() || !contract().contract()->is_partnership()) {
    announcer_->reset(*this);
  }
}

void ForwardingAi::play_start_hook()
{
  FateAi::play_start_hook();

  if (auto_play_) {
    player_ = pick_auto_ai();
  }
  player_->reset(*this);
}

void ForwardingAi::trick_complete_hook()
{
  FateAi::trick_complete_hook();

  if (debug_) {
    std::cout << tricks().back() << std::endl;
  }
}

PlayAi::Ptr ForwardingAi::pick_auto_ai()
{
  enum { declarer, partner, defender } me;
  if (!offence()) {
    me = defender;
  } else if (position() == this->declarer()) {
    me = declarer;
  } else {
    me = partner;
  }
  auto contract_name = contract().contract()->bid_name();
  switch (me) {
    case declarer:
      if (contract_name == "r") {
        return PlayAi::Ptr(new ROffenceAi());
      } else if (contract_name == "t") {
        return PlayAi::Ptr(new TAi());
      } else if (contract_name == "6d") {
        return PlayAi::Ptr(new SedOffenceAi());
      } else if (contract_name == "p") {
        return PlayAi::Ptr(new POffenceAi());
      } else if (contract_name == "b") {
        return PlayAi::Ptr(new BOffenceAi());
      } else if (contract_name == "sd") {
        return PlayAi::Ptr(new SdOffenceAi());
      }
      break;
    case partner:
      if (contract_name == "r") {
        return PlayAi::Ptr(new RPartnerAi());
      }
      break;
    case defender:
      if (contract_name == "r") {
        return PlayAi::Ptr(new RDefenceAi());
      } else if (contract_name == "t") {
        return PlayAi::Ptr(new TAi());
      } else if (contract_name == "6d") {
        return PlayAi::Ptr(new SedDefenceAi());
      } else if (contract_name == "p") {
        return PlayAi::Ptr(new PDefenceAi());
      } else if (contract_name == "b") {
        return PlayAi::Ptr(new BDefenceAi());
      } else if (contract_name == "sd") {
        return PlayAi::Ptr(new SdDefenceAi());
      }
    default:
      break;
  }

  KONIG_FATAL("no auto AI for contract "<<contract_name<<" with me "<<me);
}

}}

