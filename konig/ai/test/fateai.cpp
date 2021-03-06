#include <konig/fatal.hpp>
#include <konig/ai/listenai.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

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
      assert(i < 4); // Need more logic if not on first trick
      Card card;
      if (!Card::from_string(card, plays[i])) {
        KONIG_FATAL("couldn't parse card");
      }
      ai.notify_play_card(PlayPosition(i%4), card);
    }
  }

  PlayPosition const p0(PlayPosition(0));
  PlayPosition const p1(PlayPosition(1));
  PlayPosition const p2(PlayPosition(2));
  PlayPosition const p3(PlayPosition(3));

}

BOOST_AUTO_TEST_CASE(guess_partner)
{
  boost::optional<uint8_t> choose0(0);
  ListenAi ai;
  BOOST_TEST_CHECKPOINT("guess no partner after one card");
  rig_test(
    ai,
    position_forehand,
    "H:789tJNQK D:tJNQ",
    { "r", "", "", "", "r"},
    KingCall::diamonds,
    "1 2 3", "4 5 6",
    choose0,
    "H:789",
    std::vector<std::string>(),
    {"DQ"}
  );
  BOOST_CHECK(!ai.guess_is_partner(p0));
  BOOST_CHECK(!ai.guess_is_partner(p1));
  BOOST_CHECK(!ai.guess_is_partner(p2));
  BOOST_CHECK(!ai.guess_is_partner(p3));
  BOOST_CHECK(ai.guess_is_on_my_side(p0));
  BOOST_CHECK(!ai.guess_is_on_my_side(p1));
  BOOST_CHECK(!ai.guess_is_on_my_side(p2));
  BOOST_CHECK(!ai.guess_is_on_my_side(p3));

  BOOST_TEST_CHECKPOINT("guess partner by process of elimination");
  rig_test(
    ai,
    position_forehand,
    "D:789tJ H:89tJNQK",
    {"r", "", "", "", "r"},
    KingCall::diamonds,
    "1 2 3", "4 5 6",
    choose0,
    "H:89t",
    std::vector<std::string>(),
    {"DJ", "7", "8"}
  );
  BOOST_CHECK(!ai.guess_is_partner(p0));
  BOOST_CHECK(!ai.guess_is_partner(p1));
  BOOST_CHECK(!ai.guess_is_partner(p2));
  BOOST_CHECK(ai.guess_is_partner(p3));
  BOOST_CHECK(ai.guess_is_on_my_side(p0));
  BOOST_CHECK(!ai.guess_is_on_my_side(p1));
  BOOST_CHECK(!ai.guess_is_on_my_side(p2));
  BOOST_CHECK(ai.guess_is_on_my_side(p3));

  BOOST_TEST_CHECKPOINT("guess partner by seeing him play");
  rig_test(
    ai,
    position_forehand,
    "H:789tJNQK D:tJNQ",
    {"r", "", "", "", "r"},
    KingCall::diamonds,
    "1 2 3", "4 5 6",
    choose0,
    "H:789",
    std::vector<std::string>(),
    {"DQ", "DK"}
  );
  BOOST_CHECK(!ai.guess_is_partner(p0));
  BOOST_CHECK(ai.guess_is_partner(p1));
  BOOST_CHECK(!ai.guess_is_partner(p2));
  BOOST_CHECK(!ai.guess_is_partner(p3));
  BOOST_CHECK(ai.guess_is_on_my_side(p0));
  BOOST_CHECK(ai.guess_is_on_my_side(p1));
  BOOST_CHECK(!ai.guess_is_on_my_side(p2));
  BOOST_CHECK(!ai.guess_is_on_my_side(p3));
}

BOOST_AUTO_TEST_CASE(card_tracking)
{
  Card const skus(TrumpRank::skus);
  Card const uhu(TrumpRank::uhu);
  Card const kakadu(TrumpRank::kakadu);
  ListenAi ai;
  // In solodreier we can't infer that somone who fails to rise couldn't
  rig_test(
    ai,
    position_forehand,
    "21 C:789tJNQK D:789",
    {"sd", "", "", ""},
    KingCall::invalid,
    NULL, NULL,
    boost::optional<uint8_t>(),
    NULL,
    std::vector<std::string>(),
    {"21", "20"}
  );
  BOOST_CHECK(ai.fates_of(skus).count(CardFate::held_by(p1)));

  // In trischaken we can
  rig_test(
    ai,
    position_forehand,
    "21 C:789tJNQK D:789",
    {"r", "", "", "", "t"},
    KingCall::invalid,
    NULL, NULL,
    boost::optional<uint8_t>(),
    NULL,
    std::vector<std::string>(),
    {"21", "20"}
  );
  BOOST_CHECK(!ai.fates_of(skus).count(CardFate::held_by(p1)));

  // But we cannot assume that they hold no larger trumps
  rig_test(
    ai,
    position_forehand,
    "21 C:789tJNQK D:789",
    {"r", "", "", "", "t"},
    KingCall::invalid,
    NULL, NULL,
    boost::optional<uint8_t>(),
    NULL,
    std::vector<std::string>(),
    {"21", "2"}
  );
  BOOST_CHECK(ai.fates_of(kakadu).count(CardFate::held_by(p1)));

  // Unless it's the pagat which implies no other trumps held
  rig_test(
    ai,
    position_forehand,
    "21 C:789tJNQK D:789",
    {"r", "", "", "", "t"},
    KingCall::invalid,
    NULL, NULL,
    boost::optional<uint8_t>(),
    NULL,
    std::vector<std::string>(),
    {"21", "1"}
  );
  BOOST_CHECK(!ai.fates_of(uhu).count(CardFate::held_by(p1)));

  // Similarly for suit cards
  rig_test(
    ai,
    position_forehand,
    "21 C:789tJNQK D:9tJ",
    {"r", "", "", "", "t"},
    KingCall::invalid,
    NULL, NULL,
    boost::optional<uint8_t>(),
    NULL,
    std::vector<std::string>(),
    {"D9", "DN", "D7"}
  );
  BOOST_CHECK(!ai.fates_of(Card(Suit::diamond, SuitRank::queen)).
      count(CardFate::held_by(p2)));

  // Unless there has been a trump between
  rig_test(
    ai,
    position_forehand,
    "21 C:789tJNQK D:9tJ",
    {"r", "", "", "", "t"},
    KingCall::invalid,
    NULL, NULL,
    boost::optional<uint8_t>(),
    NULL,
    std::vector<std::string>(),
    {"D9", "1", "D7"}
  );
  BOOST_CHECK(ai.fates_of(Card(Suit::diamond, SuitRank::queen)).
      count(CardFate::held_by(p2)));
}

}}

