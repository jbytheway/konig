#include <konig/replayoracle.hpp>

#include <boost/assign/list_of.hpp>

#include <konig/ruleset.hpp>
#include <konig/play_game.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using boost::assign::list_of;

namespace konig {

BOOST_AUTO_TEST_CASE(s_outcomes_possible)
{
  Ruleset const rules = Ruleset::cheltenham();

  {
    ReplayOracle oracle(
      rules,
      {"r", "s", "", "", ""},
      "C",
      {},
      "",
      {"", "x", "", "", ""},
      {
        "H9", "H8", "H7", " 3",
        "CN", "C9", " 2", "C7",
        "S7", " 5", "SJ", "SK",
        " 1", "CJ", " 6", "Ct",
        " 8", "DQ", "D7", " 4",
        "HN", "HJ", "11", " 7",
        "10", "21", "D8", "20",
        "14", " 9", "19", "SQ",
        "15", "17", "Dt", "S8",
        "16", "12", "Sk", "CQ",
        "S9", "13", "DN", "St",
        "HK", "18", "DK", "SN"
      }
    );
    auto result = play_game(rules, oracle);
    BOOST_CHECK_EQUAL(result.outcome.string(), "stx");
    BOOST_CHECK_EQUAL(result.scores[0], -4);
    BOOST_CHECK(result.scores == list_of(-4)(12)(-4)(-4));
  }
}

BOOST_AUTO_TEST_CASE(d_outcomes_possible)
{
  Ruleset const rules = Ruleset::cheltenham();

  {
    ReplayOracle oracle(
      rules,
      {"r", "", "d", "", ""},
      "",
      {"18 SK DJ", "H:KN St"},
      "",
      {"", "", "", ""},
      {
        "H7", "H9", " 7", "HQ",
        "DK", " 1", "D8", "D7",
        " 6", "HJ", "12", "13",
        "19", "S7", "S9", "SJ",
        "Dt", " 2", "DJ", "D9",
        "20", "SN", "S8", " 3",
        "DN", " 4", "15", " 5",
        "C7", "11", "Sk", " 8",
        "C9", "17", "21", " 9",
        "Ct", "Ht", "18", "10",
        "CN", "CJ", "16", "14",
        "CQ", "CK", "SK", "SQ"
      }
    );
    auto result = play_game(rules, oracle);
    BOOST_CHECK_EQUAL(result.outcome.string(), "d");
    BOOST_CHECK(result.scores == list_of(-3)(-3)(9)(-3));
  }
}

BOOST_AUTO_TEST_CASE(t_outcomes_possible)
{
  Ruleset const rules = Ruleset::cheltenham();

  {
    ReplayOracle oracle(
      rules,
      {"r", "", "", "", "t"},
      "",
      {},
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
    auto result = play_game(rules, oracle);
    BOOST_CHECK_EQUAL(result.outcome.string(), "t4");
    BOOST_CHECK(result.scores == list_of(0)(0)(0)(0));
  }
}

}

