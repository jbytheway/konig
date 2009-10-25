#include <boost/assign/list_of.hpp>

#include <konig/biddingsequence.hpp>
#include <konig/ai/specificplayai.hpp>
#include <konig/ai/invalidplayerror.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace konig;
using boost::assign::list_of;

namespace {
  void do_bid_test(std::vector<std::string> const& bid_names)
  {
    Ruleset rules = Ruleset::cheltenham();
    Contracts const& contracts = rules.contracts();
    BiddingSequence bs(contracts);
    std::array<std::vector<Bid>, 4> bids;
    for (size_t i=0; i<bid_names.size(); ++i) {
      bids[i%4].push_back(contracts.index_by_bid_name(bid_names[i]));
    }
    std::vector<Player::Ptr> players;
    BOOST_FOREACH(auto& bid, bids) {
      players.push_back(Player::Ptr(new ai::SpecificPlayAi(std::move(bid))));
    }
    // Fake the game start
    for (PlayPosition p = position_forehand; p != position_max; ++p) {
      players[p]->start_game(rules, p, Cards());
    }
    bs.get_bids(players);
  }
}

BOOST_AUTO_TEST_CASE(bidding_failures)
{
  BOOST_CHECK_THROW(do_bid_test(list_of("t")), ai::InvalidPlayError);
}
