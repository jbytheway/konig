#include <boost/assign/list_of.hpp>

#include <konig/biddingsequence.hpp>
#include <konig/ai/specificplayai.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace konig;

BOOST_AUTO_TEST_CASE(bidding_failures)
{
  Ruleset rules = Ruleset::cheltenham();
  Contracts const& contracts = rules.contracts();

  {
    BiddingSequence bs(contracts);
    std::vector<Player::Ptr> players;
    players.push_back(Player::Ptr(new ai::SpecificPlayAi(
            std::vector<Bid>(1, contracts.index_by_bid_name("t"))
          )));
    players.push_back(Player::Ptr(new ai::SpecificPlayAi()));
    players.push_back(Player::Ptr(new ai::SpecificPlayAi()));
    players.push_back(Player::Ptr(new ai::SpecificPlayAi()));
    //bs.get_bids(players);
  }
}

