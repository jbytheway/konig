#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>

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
    std::vector<std::string> const& plays
  )
  {
    Ruleset rules = Ruleset::cheltenham();
    Contracts const& contracts = rules.contracts();
    std::array<std::vector<Bid>, 4> bids;
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
    for (size_t i=0; i<plays.size(); ++i) {
      Card c;
      assert(Card::from_string(c, plays[i]));
      play_seqs[i%4].push_back(c);
      hands[i%4].insert(c);
    }
    std::vector<Player::Ptr> players;
    for (size_t i=0; i<4; ++i) {
      BidAi::Ptr bidder(new ai::SpecificBidsAi(std::move(bids[i])));
      AnnouncementAi::Ptr announcer(new SpecificAnnouncementsAi());
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

BOOST_AUTO_TEST_CASE(outcomes_possible)
{
  {
    auto result = do_outcome_test(
      list_of("7 4 H:8")("2 D:9 H:J"),
      list_of("r")("")("")("")("t"),
      list_of
        ("21")("Sk")("20")("18")
        ("C9")("CQ")("CJ")("CN")
        ("12")("16")("19")("17")
        ("10")("14")("13")("15")
        (" 8")("CK")(" 9")("11")
        ("SJ")("St")("S8")("SN")
        ("HK")("H9")(" 5")("HQ")
        (" 6")("DN")(" 3")("DK")
        ("DQ")("DJ")("Dt")("SK")
        ("Ht")("H7")(" 1")("HN")
        ("C8")("S9")("C7")("Ct")
        ("D8")("S7")("D7")("SQ")
    );
    BOOST_CHECK(result.scores == list_of(0)(0)(0)(0));
    BOOST_CHECK_EQUAL(result.outcome.string(), "t4");
  }
  {
    auto result = do_outcome_test(
      list_of("21 15 7")("12 5 C:8"),
      list_of("r")("")("")("")("t"),
      list_of
        ("D9")("DQ")("DN")("Dt")
        ("20")("18")("Sk")("19")
        ("HQ")("Ht")("HN")("H9")
        ("14")("16")("11")("17")
        ("C9")("CJ")("Ct")("C7")
        ("13")(" 8")("10")(" 9")
        (" 4")(" 3")(" 6")(" 2")
        ("H8")("SK")("HK")("CK")
        ("DK")("SQ")("DJ")("D8")
        ("CQ")("SN")("HJ")("SJ")
        ("CN")("St")("S9")("D7")
        (" 1")("S8")("H7")("S7")
    );
    BOOST_CHECK(result.scores == list_of(-6)(2)(2)(2));
    BOOST_CHECK_EQUAL(result.outcome.string(), "tx");
  }
  {
    auto result = do_outcome_test(
      list_of("Sk 9 C:Q")("20 8 D:J"),
      list_of("r")("")("")("")("t"),
      list_of
        ("18")("19")(" 7")("21")
        ("14")("15")(" 4")("17")
        ("10")("11")("DK")("16")
        (" 2")(" 6")("DQ")("13")
        (" 1")(" 5")("SQ")("12")
        ("H8")("H9")("HJ")("HN")
        ("H7")("CK")("Ht")("HK")
        ("CJ")("SK")("DN")("HQ")
        ("Ct")("SJ")("St")("SN")
        ("Dt")("CN")("S8")("S9")
        ("D9")("C9")("C8")("S7")
        ("C7")("D8")("D7")(" 3")
    );
    BOOST_CHECK(result.scores == list_of(1)(1)(1)(-3));
    BOOST_CHECK_EQUAL(result.outcome.string(), "t3+");
  }
}

}}

