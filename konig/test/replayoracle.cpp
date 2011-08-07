#include <konig/replayoracle.hpp>

#include <boost/assign/list_of.hpp>

#include <konig/ruleset.hpp>
#include <konig/play_game.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using boost::assign::list_of;

namespace konig {

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

