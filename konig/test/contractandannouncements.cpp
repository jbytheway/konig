#include <konig/contractandannouncements.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/lexical_cast.hpp>

using namespace konig;

BOOST_AUTO_TEST_CASE(contracts_correctly_output)
{
  {
    Contract::ConstPtr contract = Contract::solodreier();
    ContractAndAnnouncements ca(contract);
    BOOST_CHECK_EQUAL(ca.string(1), "sd");
    ca.add({Announcement(Feat::game, Announcedness::kontraed, false)});
    BOOST_CHECK_EQUAL(ca.string(1), "sdx");
    ca.add({Announcement(Feat::forty_five, Announcedness::announced, true)});
    BOOST_CHECK_EQUAL(ca.string(1), "sdxf!");
  }
  {
    Contract::ConstPtr contract = Contract::trischaken();
    ContractAndAnnouncements ca(contract);
    BOOST_CHECK_EQUAL(ca.string(1), "t");
  }
}

