#include <konig/outcome.hpp>
#include <konig/contract.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/lexical_cast.hpp>

using namespace konig;

BOOST_AUTO_TEST_CASE(outcomes_correctly_output)
{
  Contract::ConstPtr contract = Contract::solodreier();

  {
    Outcome o(contract, 1);
    o.add(true, Feat::game, Announcedness::announced, Achievement::made);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd");
  }
  {
    Outcome o(contract, 1);
    o.add(true, Feat::game, Announcedness::announced, Achievement::off);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd/");
  }
  {
    Outcome o(contract, 1);
    o.add(true, Feat::game, Announcedness::kontraed, Achievement::made);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sdx");
  }
  {
    Outcome o(contract, 1);
    o.add(true, Feat::game, Announcedness::kontraed, Achievement::off);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sdx/");
  }
  {
    Outcome o(contract, 1);
    o.add(true, Feat::game, Announcedness::announced, Achievement::made);
    o.add(true, Feat::pagat, Announcedness::unannounced, Achievement::made);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd1");
  }
}

BOOST_AUTO_TEST_CASE(outcomes_correctly_score)
{
  Contract::ConstPtr contract = Contract::solodreier();
  std::array<bool, 4> achievers{{true, false, false, false}};
  typedef std::array<int, 4> Scores;

  {
    Outcome o(contract, 1);
    o.add(true, Feat::game, Announcedness::announced, Achievement::made);
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{18, -6, -6, -6}}));
  }
  {
    Outcome o(contract, 1);
    o.add(true, Feat::game, Announcedness::announced, Achievement::off);
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{-18, 6, 6, 6}}));
  }
  {
    Outcome o(contract, 1);
    o.add(true, Feat::game, Announcedness::kontraed, Achievement::made);
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{36, -12, -12, -12}}));
  }
  {
    Outcome o(contract, 1);
    o.add(true, Feat::game, Announcedness::kontraed, Achievement::off);
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{-36, 12, 12, 12}}));
  }
  {
    Outcome o(contract, 1);
    o.add(true, Feat::game, Announcedness::announced, Achievement::made);
    o.add(true, Feat::pagat, Announcedness::unannounced, Achievement::made);
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{24, -8, -8, -8}}));
  }
}

