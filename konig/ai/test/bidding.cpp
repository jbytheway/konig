#include <boost/assign/list_of.hpp>

#include <konig/biddingsequence.hpp>
#include <konig/ai/specificbidsai.hpp>
#include <konig/ai/specificannouncementsai.hpp>
#include <konig/ai/specificplayai.hpp>
#include <konig/ai/forwardingai.hpp>
#include <konig/ai/invaliderror.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

namespace konig { namespace ai {

using boost::assign::list_of;

namespace {
  std::string do_bid_test(std::vector<std::string> const& bid_names)
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
      BidAi::Ptr bidder(new SpecificBidsAi(std::move(bid)));
      AnnouncementAi::Ptr announcer(new SpecificAnnouncementsAi());
      PlayAi::Ptr player(new SpecificPlayAi());
      players.push_back(Player::Ptr(
          new ForwardingAi(bidder, announcer, player)
      ));
    }
    // Fake the game start
    Cards fake_hand = Cards::from_string("C:789tJNQK D:789t");
    for (PlayPosition p = position_forehand; p != position_max; ++p) {
      players[p]->start_game(rules, p, fake_hand);
    }
    return bs.get_bids(players).get<0>()->bid_name();
  }
}

BOOST_AUTO_TEST_CASE(bidding_failures)
{
  BOOST_CHECK_THROW(do_bid_test(list_of("")), ai::InvalidPlayError);
  BOOST_CHECK_THROW(do_bid_test(list_of("r")), ai::InvalidPlayError);
  BOOST_CHECK_THROW(do_bid_test(list_of("t")), ai::InvalidPlayError);
  BOOST_CHECK_THROW(do_bid_test(list_of("6d")), ai::InvalidPlayError);
  BOOST_CHECK_THROW(do_bid_test(list_of("r")("")("")("")
                                       ("s")), ai::InvalidPlayError);
  BOOST_CHECK_THROW(do_bid_test(list_of("r")("s")("")("")
                                       ("t")), ai::InvalidPlayError);
  BOOST_CHECK_THROW(do_bid_test(list_of("r")("t")("")("")
                                       ("t")), ai::InvalidPlayError);
  BOOST_CHECK_THROW(do_bid_test(list_of("p")("s")("")("")
                                       ("")), ai::InvalidPlayError);
  BOOST_CHECK_THROW(do_bid_test(list_of("r")("p")("")("")
                                       ("s")), ai::InvalidPlayError);
  BOOST_CHECK_THROW(do_bid_test(list_of("r")("r")("")("")
                                       ("")), ai::InvalidPlayError);
}

BOOST_AUTO_TEST_CASE(bidding_results)
{
  BOOST_CHECK_EQUAL("r",  do_bid_test(list_of("r")("")("")("")
                                             ("r")));
  BOOST_CHECK_EQUAL("t",  do_bid_test(list_of("r")("")("")("")
                                             ("t")));
  BOOST_CHECK_EQUAL("6d", do_bid_test(list_of("r")("")("")("")
                                             ("6d")));
  BOOST_CHECK_EQUAL("s",  do_bid_test(list_of("r")("s")("")("")
                                             ("")));
  BOOST_CHECK_EQUAL("s",  do_bid_test(list_of("r")("s")("")("")
                                             ("s")));
  BOOST_CHECK_EQUAL("sd", do_bid_test(list_of("sd")("")("")("")));
}

}}

