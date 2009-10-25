#include <konig/outcome.hpp>
#include <konig/contract.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>

using namespace konig;

BOOST_AUTO_TEST_CASE(outcomess_correctly_output)
{
  Contract::ConstPtr contract = Contract::solodreier();

  {
    Outcome o(contract);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd");
    o.add(true, Feat::game, Announcedness::announced, Achievement::made);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd");
    o.add(true, Feat::game, Announcedness::announced, Achievement::off);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd/");
    o.add(true, Feat::game, Announcedness::kontraed, Achievement::made);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sdx");
    o.add(true, Feat::game, Announcedness::kontraed, Achievement::off);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sdx/");
  }
}

