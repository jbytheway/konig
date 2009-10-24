#include <konig/contractandannouncements.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>

using namespace konig;

BOOST_AUTO_TEST_CASE(contracts_correctly_output)
{
  Contract::ConstPtr contract = Contract::solodreier();

  {
    ContractAndAnnouncements ca(contract);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(ca), "sd");
    ca.add(boost::assign::list_of(
          Announcement(Feat::game, Announcedness::kontraed, false)
        ));
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(ca), "sdx");
    ca.add(boost::assign::list_of(
          Announcement(Feat::forty_five, Announcedness::announced, true)
        ));
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(ca), "sdxf!");
  }
}

