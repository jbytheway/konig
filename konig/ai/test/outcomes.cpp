#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

#include <konig/utility/intersects.hpp>
#include <konig/ai/specificbidsai.hpp>
#include <konig/ai/specificannouncementsai.hpp>
#include <konig/ai/specificplayai.hpp>
#include <konig/ai/forwardingai.hpp>
#include <konig/game.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using boost::assign::list_of;

namespace konig { namespace ai {

namespace {
  PlayResult do_outcome_test(
    std::vector<std::string> const& talon_string,
    std::vector<std::string> const& bid_names,
    std::string const& king_call_name,
    std::string const& discard_names,
    std::vector<std::string> const& announcement_names,
    std::vector<std::string> const& plays
  )
  {
    Ruleset rules = Ruleset::cheltenham();
    Contracts const& contracts = rules.contracts();
    std::array<std::vector<Bid>, 4> bids;
    boost::optional<KingCall> king_call;
    bool concession = false;
    int talon_choice = -1;
    Cards discard;
    std::array<std::vector<std::vector<Announcement>>, 4> announcements;
    std::array<std::vector<Card>, 4> play_seqs;
    std::array<Cards, 4> hands;
    assert(talon_string.size() == 2);
    std::array<Cards, 2> talon{{
      Cards::from_string(talon_string[0]),
      Cards::from_string(talon_string[1])
    }};

    for (size_t i=0; i<bid_names.size(); ++i) {
      bids[i%4].push_back(contracts.index_by_bid_name(bid_names[i]));
    }

    if (!king_call_name.empty()) {
      king_call = KingCall();
      bool result = KingCall::from_string(*king_call, king_call_name);
      BOOST_CHECK(result);
    }

    discard = Cards::from_string(discard_names);

    if (announcement_names == std::vector<std::string>{"c"}) {
      concession = true;
    } else {
      for (size_t i=0; i<announcement_names.size(); ++i) {
        std::vector<Announcement> these_announcements;
        bool result = Announcement::many_from_string(
          these_announcements, announcement_names[i]
        );
        BOOST_CHECK(result);
        announcements[i%4].push_back(these_announcements);
      }
    }

    for (size_t i=0; i<plays.size(); ++i) {
      Card c;
      assert(Card::from_string(c, plays[i]));
      play_seqs[i%4].push_back(c);
      hands[i%4].insert(c);
    }

    // Declarer's hand may be wrong if he is supposed to exchange with the
    // talon
    // FIXME: this code doesn't necessarily work if the discard is 6 cards
    for (size_t i=0; i<4; ++i) {
      auto& hand = hands[i];
      for (size_t talon_half=0; talon_half<2; ++talon_half) {
        if (utility::intersects(talon[talon_half], hand)) {
          talon_choice = talon_half;
          hand.insert(discard);
          hand.erase(talon[talon_half]);
        }
      }
      BOOST_CHECK_EQUAL(hand.size(), 12);
    }

    std::vector<Player::Ptr> players;
    for (size_t i=0; i<4; ++i) {
      BidAi::Ptr bidder(new ai::SpecificBidsAi(std::move(bids[i])));
      AnnouncementAi::Ptr announcer(
        new SpecificAnnouncementsAi(
          announcements[i], king_call, talon_choice, discard,
          concession
        )
      );
      PlayAi::Ptr player(new ai::SpecificPlayAi(std::move(play_seqs[i])));
      players.push_back(Player::Ptr(
          new ForwardingAi(bidder, announcer, player)
      ));
    }
    Deal deal(hands, talon);
    Game game(rules, players, deal);
    return game.play();
  }
}

BOOST_AUTO_TEST_CASE(t_outcomes_possible)
{
  {
    auto result = do_outcome_test(
      {"7 4 H:8", "2 D:9 H:J"},
      {"r", "", "", "", "t"},
      "",
      "",
      {},
      {
        "21", "Sk", "20", "18",
        "C9", "CQ", "CJ", "CN",
        "12", "16", "19", "17",
        "10", "14", "13", "15",
        " 8", "CK", " 9", "11",
        "SJ", "St", "S8", "SN",
        "HK", "H9", " 5", "HQ",
        " 6", "DN", " 3", "DK",
        "DQ", "DJ", "Dt", "SK",
        "Ht", "H7", " 1", "HN",
        "C8", "S9", "C7", "Ct",
        "D8", "S7", "D7", "SQ"
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "t4");
    BOOST_CHECK(result.scores == list_of(0)(0)(0)(0));
  }
  {
    auto result = do_outcome_test(
      {"21 15 7", "12 5 C:8"},
      {"r", "", "", "", "t"},
      "",
      "",
      {},
      {
        "D9", "DQ", "DN", "Dt",
        "20", "18", "Sk", "19",
        "HQ", "Ht", "HN", "H9",
        "14", "16", "11", "17",
        "C9", "CJ", "Ct", "C7",
        "13", " 8", "10", " 9",
        " 4", " 3", " 6", " 2",
        "H8", "SK", "HK", "CK",
        "DK", "SQ", "DJ", "D8",
        "CQ", "SN", "HJ", "SJ",
        "CN", "St", "S9", "D7",
        " 1", "S8", "H7", "S7"
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "tx");
    BOOST_CHECK(result.scores == list_of(-6)(2)(2)(2));
  }
  {
    auto result = do_outcome_test(
      {"Sk 9 C:Q", "20 8 D:J"},
      {"r", "", "", "", "t"},
      "",
      "",
      {},
      {
        "18", "19", " 7", "21",
        "14", "15", " 4", "17",
        "10", "11", "DK", "16",
        " 2", " 6", "DQ", "13",
        " 1", " 5", "SQ", "12",
        "H8", "H9", "HJ", "HN",
        "H7", "CK", "Ht", "HK",
        "CJ", "SK", "DN", "HQ",
        "Ct", "SJ", "St", "SN",
        "Dt", "CN", "S8", "S9",
        "D9", "C9", "C8", "S7",
        "C7", "D8", "D7", " 3"
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "t3+");
    BOOST_CHECK(result.scores == list_of(1)(1)(1)(-3));
  }
}

BOOST_AUTO_TEST_CASE(concession_possible)
{
  {
    auto result = do_outcome_test(
      {"C:K D:Kt", "4 C:8 D:Q"},
      {"r", "", "", "", "r"},
      "C",
      "D:QJ9",
      {},
      {
        "C8", "Ct", "CJ", "C9",
        "CN", " 2", "C7", " 1",
        "H7", "Ht", "H8", " 5",
        "SK", "SQ", " 6", "S7",
        "HJ", "HK", "H9", " 7",
        " 3", " 9", " 8", "S8",
        "HQ", "HN", "15", "10",
        " 4", "D7", "DN", "18",
        "11", "12", "16", "S9",
        "13", "21", "17", "St",
        "14", "D8", "19", "SJ",
        "CQ", "Sk", "20", "SN"
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "rt/f!v!");
    BOOST_CHECK(result.scores == list_of(-15)(5)(5)(5));
  }
  {
    auto result = do_outcome_test(
      {"C:K D:Kt", "4 C:8 D:Q"},
      {"r", "", "", "", "r"},
      "C",
      "D:QJ9",
      {"c"}, // Concession
      {
        "C8", "Ct", "CJ", "C9",
        "CN", " 2", "C7", " 1",
        "H7", "Ht", "H8", " 5",
        "SK", "SQ", " 6", "S7",
        "HJ", "HK", "H9", " 7",
        " 3", " 9", " 8", "S8",
        "HQ", "HN", "15", "10",
        " 4", "D7", "DN", "18",
        "11", "12", "16", "S9",
        "13", "21", "17", "St",
        "14", "D8", "19", "SJ",
        "CQ", "Sk", "20", "SN"
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "rtc");
    BOOST_CHECK(result.scores == list_of(-3)(1)(1)(1));
  }
}

BOOST_AUTO_TEST_CASE(s_scoring_rules)
{
  // Solo against three
  {
    auto result = do_outcome_test(
      {"10 C:Q D:7", "4 D:K9"},
      {"s", "", "", ""},
      "D",
      "",
      {"", "", "", ""},
      {
        "C9", "C7", "C8", "Ct",
        "SK", "SJ", "St", "S8",
        "CJ", " 3", " 1", " 9",
        "S7", " 6", " 5", "S9",
        " 2", "HJ", "H7", "H8",
        "CN", " 7", "12", "11",
        "18", "HN", "H9", "Ht",
        "CK", " 8", "13", "14",
        "SQ", "16", "15", "SN",
        "Sk", "HQ", "HK", "17",
        "DN", "Dt", "19", "D8",
        "DQ", "DJ", "20", "21"
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "st/");
    BOOST_CHECK(result.scores == list_of(-6)(2)(2)(2));
  }
  {
    auto result = do_outcome_test(
      {"10 C:Q D:7", "4 D:K9"},
      {"s", "", "", ""},
      "D",
      {},
      {"", "x", "", "", ""},
      {
        "C9", "C7", "C8", "Ct",
        "SK", "SJ", "St", "S8",
        "CJ", " 3", " 1", " 9",
        "S7", " 6", " 5", "S9",
        " 2", "HJ", "H7", "H8",
        "CN", " 7", "12", "11",
        "18", "HN", "H9", "Ht",
        "CK", " 8", "13", "14",
        "SQ", "16", "15", "SN",
        "Sk", "HQ", "HK", "17",
        "DN", "Dt", "19", "D8",
        "DQ", "DJ", "20", "21"
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "stx/");
    // Kontra is cancelled
    BOOST_CHECK_EQUAL(result.scores[0], -6);
    BOOST_CHECK(result.scores == list_of(-6)(2)(2)(2));
  }
  {
    auto result = do_outcome_test(
      {"10 C:Q D:7", "4 D:K9"},
      {"s", "", "", ""},
      "D",
      {},
      {"k", "x", "", "", ""},
      {
        "C9", "C7", "C8", "Ct",
        "SK", "SJ", "St", "S8",
        "CJ", " 3", " 1", " 9",
        "S7", " 6", " 5", "S9",
        " 2", "HJ", "H7", "H8",
        "CN", " 7", "12", "11",
        "18", "HN", "H9", "Ht",
        "CK", " 8", "13", "14",
        "SQ", "16", "15", "SN",
        "Sk", "HQ", "HK", "17",
        "DN", "Dt", "19", "D8",
        "DQ", "DJ", "20", "21"
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "stx/k-");
    // Kontra not cancelled if declarer announced something else
    BOOST_CHECK(result.scores == list_of(-24)(8)(8)(8));
  }
  {
    auto result = do_outcome_test(
      {"10 C:Q D:7", "4 D:K9"},
      {"s", "", "", ""},
      "D",
      {},
      {
        "", "x", "", "",
        "xx"
      },
      {
        "C9", "C7", "C8", "Ct",
        "SK", "SJ", "St", "S8",
        "CJ", " 3", " 1", " 9",
        "S7", " 6", " 5", "S9",
        " 2", "HJ", "H7", "H8",
        "CN", " 7", "12", "11",
        "18", "HN", "H9", "Ht",
        "CK", " 8", "13", "14",
        "SQ", "16", "15", "SN",
        "Sk", "HQ", "HK", "17",
        "DN", "Dt", "19", "D8",
        "DQ", "DJ", "20", "21"
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "stxx/");
    // Kontra not cancelled if declarer rekontraed
    BOOST_CHECK(result.scores == list_of(-24)(8)(8)(8));
  }
  {
    auto result = do_outcome_test(
      {"7 C:7 S:K", "19 D:t S:t"},
      {"s", "", "", ""},
      "S",
      {},
      {"", "x", "", "", ""},
      {
        "12", " 6", "14", " 2",
        " 4", "11", "D8", "D7",
        "CJ", "C8", "Ct", "C9",
        "13", "S8", "16", " 5",
        "Sk", "S9", "D9", "DQ",
        "CQ", "CK", "CN", "10",
        "H8", "HK", " 3", "H7",
        "20", "Ht", "DJ", "17",
        "HN", "HQ", " 8", "H9",
        "15", "SN", "DN", "18",
        "SQ", "SJ", " 9", "S7",
        " 1", "HJ", "DK", "21",
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "stx/1/f!");
    // Kontra *is* cancelled if declarer did something else unannounced
    BOOST_CHECK(result.scores == list_of(-12)(4)(4)(4));
  }
  // Solo with a partner
  {
    auto result = do_outcome_test(
      {"D:7 S:N8", "21 14 13"},
      {"s", "", "", ""},
      "D",
      {},
      {"", "", "", ""},
      {
        "C9", "CQ", "C7", "C8",
        "DN", "D8", " 4", "DJ",
        " 2", "HK", "H8", "HQ",
        "CJ", " 1", " 7", "Ct",
        "18", "H7", "Ht", " 3",
        "CN", "17", "11", " 5",
        "DQ", "D9", "12", " 6",
        "Sk", "H9", "HJ", " 8",
        "CK", "19", "15", " 9",
        "S7", "Dt", "16", "10",
        "SQ", "DK", "HN", "SJ",
        "SK", "S9", "20", "St",
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "s/");
    BOOST_CHECK(result.scores == list_of(-2)(-2)(2)(2));
  }
  {
    auto result = do_outcome_test(
      {"D:7 S:N8", "21 14 13"},
      {"s", "", "", ""},
      "D",
      {},
      {"", "", "x", "", ""},
      {
        "C9", "CQ", "C7", "C8",
        "DN", "D8", " 4", "DJ",
        " 2", "HK", "H8", "HQ",
        "CJ", " 1", " 7", "Ct",
        "18", "H7", "Ht", " 3",
        "CN", "17", "11", " 5",
        "DQ", "D9", "12", " 6",
        "Sk", "H9", "HJ", " 8",
        "CK", "19", "15", " 9",
        "S7", "Dt", "16", "10",
        "SQ", "DK", "HN", "SJ",
        "SK", "S9", "20", "St",
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "sx/");
    BOOST_CHECK(result.scores == list_of(-4)(-4)(4)(4));
  }
}

BOOST_AUTO_TEST_CASE(s_king_off)
{
  {
    auto result = do_outcome_test(
      {"17 10 H:8", "H:Q S:J9"},
      {"s", "", "", ""},
      "D",
      {},
      {"", "", "", ""},
      {
        "C9", "CQ", "C8", "C7",
        " 2", "Ht", "H9", "H7",
        "CJ", " 1", "Ct", " 3",
        "18", "HK", "HN", "HJ",
        "CN", " 7", " 9", " 4",
        "DQ", " 8", "D7", "D8",
        "S7", "S8", "12", "SN",
        "DN", "11", "D9", " 5",
        "SQ", "St", "16", " 6",
        "Sk", "13", "Dt", "14",
        "CK", "15", "DJ", "19",
        "SK", "20", "DK", "21",
      }
    );
    BOOST_CHECK_EQUAL(result.outcome.string(), "s/k/");
    BOOST_CHECK(result.scores == list_of(-4)(4)(-4)(4));
  }
}

}}

