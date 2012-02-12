#include <konig/outcome.hpp>
#include <konig/contract.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/lexical_cast.hpp>

using namespace konig;

BOOST_AUTO_TEST_CASE(outcomes_correctly_output)
{
  Contract::ConstPtr contract = Contract::solodreier();
  // Technically it's invalid to pass an empty announcednesses to these
  // functions; it should include all the ones we're scoring, but except for
  // Solo it doesn't actually matter.
  Announcednesses announcednesses;

  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::announced, Achievement::made,
      true, announcednesses
    );
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd");
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::announced, Achievement::off,
      true, announcednesses
    );
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd/");
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::kontraed, Achievement::made,
      true, announcednesses
    );
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sdx");
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::kontraed, Achievement::off,
      true, announcednesses
    );
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sdx/");
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::announced, Achievement::made,
      true, announcednesses
    );
    o.add(
      true, Feat::pagat, Announcedness::unannounced, Achievement::made,
      true, announcednesses
    );
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd1");
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::announced, Achievement::off,
      true, announcednesses
    );
    o.add(
      true, Feat::uhu, Announcedness::unannounced, Achievement::off,
      true, announcednesses
    );
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd/2/");
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::announced, Achievement::off,
      true, announcednesses
    );
    o.add(
      true, Feat::uhu, Announcedness::unannounced, Achievement::off,
      true, announcednesses
    );
    o.add(
      false, Feat::forty_five, Announcedness::unannounced, Achievement::made,
      true, announcednesses
    );
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(o), "sd/2/f!");
  }
}

BOOST_AUTO_TEST_CASE(outcomes_correctly_score)
{
  Contract::ConstPtr contract = Contract::solodreier();
  std::array<bool, 4> achievers{{true, false, false, false}};
  typedef std::array<int, 4> Scores;
  // Technically it's invalid to pass an empty announcednesses to these
  // functions; it should include all the ones we're scoring, but except for
  // Solo it doesn't actually matter.
  Announcednesses announcednesses;

  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::announced, Achievement::made,
      true, announcednesses
    );
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{18, -6, -6, -6}}));
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::announced, Achievement::off,
      true, announcednesses
    );
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{-18, 6, 6, 6}}));
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::kontraed, Achievement::made,
      true, announcednesses
    );
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{36, -12, -12, -12}}));
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::kontraed, Achievement::off,
      true, announcednesses
    );
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{-36, 12, 12, 12}}));
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::announced, Achievement::made,
      true, announcednesses
    );
    o.add(
      true, Feat::pagat, Announcedness::unannounced, Achievement::made,
      true, announcednesses
    );
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{24, -8, -8, -8}}));
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::announced, Achievement::off,
      true, announcednesses
    );
    o.add(
      true, Feat::uhu, Announcedness::unannounced, Achievement::off,
      true, announcednesses
    );
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{-24, 8, 8, 8}}));
  }
  {
    Outcome o(*contract, 1);
    o.add(
      true, Feat::game, Announcedness::announced, Achievement::off,
      true, announcednesses
    );
    o.add(
      true, Feat::uhu, Announcedness::unannounced, Achievement::off,
      true, announcednesses
    );
    o.add(
      false, Feat::forty_five, Announcedness::unannounced, Achievement::made,
      true, announcednesses
    );
    BOOST_CHECK((o.compute_scores(achievers) == Scores{{-24, 8, 8, 8}}));
  }
}

