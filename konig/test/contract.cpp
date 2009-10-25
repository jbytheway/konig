#include <konig/contract.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace konig;

BOOST_AUTO_TEST_CASE(contract_names_correct)
{
  {
    Contract::ConstPtr contract = Contract::solodreier();
    BOOST_CHECK_EQUAL(contract->bid_name(), "sd");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::announced, Achievement::made
        ), "sd");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::announced, Achievement::off
        ), "sd/");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::kontraed, Achievement::made
        ), "sdx");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::kontraed, Achievement::off
        ), "sdx/");
  }
  {
    Contract::ConstPtr contract = Contract::rufer();
    BOOST_CHECK_EQUAL(contract->bid_name(), "r");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          2, Announcedness::announced, Achievement::made
        ), "r");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          2, Announcedness::announced, Achievement::off
        ), "r/");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          2, Announcedness::kontraed, Achievement::made
        ), "rx");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          2, Announcedness::kontraed, Achievement::off
        ), "rx/");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::announced, Achievement::made
        ), "r3");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::announced, Achievement::off
        ), "r3/");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::kontraed, Achievement::made
        ), "r3x");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::kontraed, Achievement::off
        ), "r3x/");
  }
  {
    Contract::ConstPtr contract = Contract::bettel();
    BOOST_CHECK_EQUAL(contract->bid_name(), "b");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::announced, Achievement::made
        ), "b");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::announced, Achievement::off
        ), "b/");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::kontraed, Achievement::made
        ), "bx");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::kontraed, Achievement::off
        ), "bx/");
  }
  {
    Contract::ConstPtr contract = Contract::trischaken();
    BOOST_CHECK_EQUAL(contract->bid_name(), "t");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::announced, Achievement::made
        ), "t");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          1, Announcedness::announced, Achievement::off
        ), "t+");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          2, Announcedness::announced, Achievement::made
        ), "t2");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          3, Announcedness::announced, Achievement::made
        ), "t3");
    BOOST_CHECK_EQUAL(contract->outcome_name(
          4, Announcedness::announced, Achievement::made
        ), "t4");
  }
}

