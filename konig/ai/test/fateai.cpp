#include <boost/assign/list_of.hpp>

#include <konig/fatal.hpp>
#include <konig/ai/listenai.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using boost::assign::list_of;

namespace konig { namespace ai {

namespace {
  void rig_test(
    FateAi& ai,
    PlayPosition const ai_pos,
    std::string const& hand_s,
    std::vector<std::string> const& bid_names,
    KingCall const king_call,
    char const* const talon0,
    char const* const talon1,
    boost::optional<uint8_t> const& talon_choice,
    char const* const discard,
    std::vector<std::string> const& announcements,
    std::vector<std::string> const& plays
  )
  {
    Ruleset rules = Ruleset::cheltenham();
    Contracts const& contracts = rules.contracts();
    Cards hand = Cards::from_string(hand_s);
    ai.start_game(rules, ai_pos, hand);
    Bid last_bid = Bid::pass;
    for (size_t i=0; i<bid_names.size(); ++i) {
      auto bid = contracts.index_by_bid_name(bid_names[i]);
      if (!bid.is_pass()) last_bid = bid;
      ai.notify_bid(PlayPosition(i%4), bid);
    }
    ai.notify_contract_established(last_bid);
    if (king_call != KingCall::invalid) ai.notify_call_king(king_call);
    if (talon0) {
      assert(talon1);
      std::array<Cards, 2> talon{{
        Cards::from_string(talon0), Cards::from_string(talon1)
      }};
      ai.notify_talon(talon);
    }
    if (talon_choice) ai.notify_talon_choice(*talon_choice);
    if (discard) {
      Cards const cards = Cards::from_string(discard);
      ai.notify_discard(cards);
    }
    for (size_t i=0; i<announcements.size(); ++i) {
      std::vector<Announcement> ans;
      if (!Announcement::many_from_string(ans, announcements[i])) {
        KONIG_FATAL("couldn't parse announcements");
      }
      ai.notify_announcements(ans);
    }
    ai.notify_announcements_done();
    for (size_t i=0; i<plays.size(); ++i) {
      Card card;
      if (!Card::from_string(card, plays[i])) {
        KONIG_FATAL("couldn't parse card");
      }
      ai.notify_play_card(PlayPosition(i%4), card);
    }
  }
}

BOOST_AUTO_TEST_CASE(guess_partner)
{
  boost::optional<uint8_t> choose0(0);
  PlayPosition p0(PlayPosition(0)), p1(PlayPosition(1));
  PlayPosition p2(PlayPosition(2)), p3(PlayPosition(3));
  ListenAi ai;
  BOOST_TEST_CHECKPOINT("guess partner after one card");
  rig_test(
    ai,
    position_forehand,
    "H:789tJNQK D:tJNQ",
    list_of("r")("")("")("")("r"),
    KingCall::diamonds,
    "1 2 3", "4 5 6",
    choose0,
    "H:789",
    std::vector<std::string>(),
    list_of("DQ")
  );
  BOOST_CHECK(!ai.guess_is_partner(p0));
  BOOST_CHECK(!ai.guess_is_partner(p1));
  BOOST_CHECK(!ai.guess_is_partner(p2));
  BOOST_CHECK(!ai.guess_is_partner(p3));
  BOOST_CHECK(ai.guess_is_on_my_side(p0));
  BOOST_CHECK(!ai.guess_is_on_my_side(p1));
  BOOST_CHECK(!ai.guess_is_on_my_side(p2));
  BOOST_CHECK(!ai.guess_is_on_my_side(p3));
}

}}

