#include <konig/replayoracle.hpp>

#include <konig/utility/intersects.hpp>
#include <konig/ruleset.hpp>
#include <konig/invaliderror.hpp>

namespace konig {

ReplayOracle::ReplayOracle(
  Ruleset const& rules,
  std::vector<std::string> const& bid_names,
  std::string const& king_call_name,
  std::vector<std::string> const& talon_string,
  std::string const& discard_names,
  std::vector<std::string> const& announcement_names,
  std::vector<std::string> const& plays
) :
  concession_(false),
  talon_choice_(-1)
{
  Contracts const& contracts = rules.contracts();

  if (!talon_string.empty()) {
    assert(talon_string.size() == 2);
    talon_ = std::array<Cards, 2>{{
      Cards::from_string(talon_string[0]),
      Cards::from_string(talon_string[1])
    }};
  }

  for (size_t i=0; i<bid_names.size(); ++i) {
    bids_[i%4].push_back(contracts.index_by_bid_name(bid_names[i]));
  }

  if (!king_call_name.empty()) {
    king_call_ = KingCall();
    bool result = KingCall::from_string(*king_call_, king_call_name);
    assert(result);
  }

  discard_ = Cards::from_string(discard_names);

  if (announcement_names == std::vector<std::string>{"c"}) {
    concession_ = true;
  } else {
    for (size_t i=0; i<announcement_names.size(); ++i) {
      std::vector<Announcement> these_announcements;
      bool result = Announcement::many_from_string(
        these_announcements, announcement_names[i]
      );
      assert(result);
      announcements_[i%4].push_back(these_announcements);
    }
  }

  std::array<Cards, 4> hands;

  for (size_t i=0; i<plays.size(); ++i) {
    Card c;
    assert(Card::from_string(c, plays[i]));
    plays_[i%4].push_back(c);
    hands[i%4].insert(c);
  }

  // Figure out which half of talon declarer took
  // FIXME: this may not work if declarer discarded exactly what he picked up
  for (size_t i=0; i<4; ++i) {
    auto& hand = hands[i];
    for (size_t talon_half=0; talon_half<2; ++talon_half) {
      using utility::intersects;
      if (intersects(talon_[talon_half], hand)) {
        talon_choice_ = talon_half;
      }
    }
  }
}

Bid ReplayOracle::bid(PlayPosition p)
{
  auto& bids = bids_[p];
  assert(!bids.empty());
  auto result = bids.front();
  bids.pop_front();
  return result;
}

KingCall ReplayOracle::call_king(PlayPosition)
{
  assert(king_call_);
  return *king_call_;
}

std::array<Cards, 2> ReplayOracle::get_talon()
{
  assert(!talon_[0].empty() && !talon_[1].empty());
  return talon_;
}

bool ReplayOracle::choose_concede(PlayPosition)
{
  return concession_;
}

uint8_t ReplayOracle::choose_talon_half(PlayPosition)
{
  assert(talon_choice_ == 0 || talon_choice_ == 1);
  return talon_choice_;
}

Cards ReplayOracle::discard(PlayPosition)
{
  return discard_;
}

std::vector<Announcement> ReplayOracle::announce(PlayPosition p)
{
  auto& announcements = announcements_[p];
  assert(!announcements.empty());
  auto result = announcements.front();
  announcements.pop_front();
  return result;
}

Card ReplayOracle::play_card(PlayPosition p)
{
  auto& plays = plays_[p];
  assert(!plays.empty());
  auto result = plays.front();
  plays.pop_front();
  return result;
}

Cards ReplayOracle::get_ouvert(PlayPosition)
{
  KONIG_FATAL("not implemented");
}

void ReplayOracle::notify_invalid(PlayPosition, std::string const& m)
{
  throw InvalidError(m);
}

void ReplayOracle::notify_bid(PlayPosition, Bid) {}
void ReplayOracle::notify_contract_established(Bid) {}
void ReplayOracle::notify_call_king(KingCall) {}
void ReplayOracle::notify_talon(std::array<Cards, 2> const& /*talon*/) {}
void ReplayOracle::notify_concede() {}
void ReplayOracle::notify_talon_choice(uint8_t) {}
void ReplayOracle::notify_discard(Cards) {}
void ReplayOracle::notify_announcements(std::vector<Announcement>) {}
void ReplayOracle::notify_announcements_done() {}
void ReplayOracle::notify_play_card(PlayPosition, Card) {}
void ReplayOracle::notify_ouvert(Cards const&) {}


}

