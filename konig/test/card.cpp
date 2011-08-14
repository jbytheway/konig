#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/lexical_cast.hpp>

#include <konig/card.hpp>

namespace konig {

BOOST_AUTO_TEST_CASE(card_parser)
{
  {
    Card c;
    BOOST_CHECK_EQUAL(true, Card::from_string(c, "Sk"));
    BOOST_CHECK_EQUAL(c, Card(TrumpRank::skus));
  }
  {
    Card c;
    BOOST_CHECK_EQUAL(true, Card::from_string(c, "21"));
    BOOST_CHECK_EQUAL(c, Card(TrumpRank::mond));
  }
  {
    Card c;
    BOOST_CHECK_EQUAL(true, Card::from_string(c, "1"));
    BOOST_CHECK_EQUAL(c, Card(TrumpRank::pagat));
  }
  {
    Card c;
    BOOST_CHECK_EQUAL(true, Card::from_string(c, " 8"));
    BOOST_CHECK_EQUAL(c, Card(TrumpRank(8)));
  }
  {
    Card c;
    BOOST_CHECK_EQUAL(true, Card::from_string(c, " 17 "));
    BOOST_CHECK_EQUAL(c, Card(TrumpRank(17)));
  }
  {
    Card c;
    BOOST_CHECK_EQUAL(true, Card::from_string(c, "HK"));
    BOOST_CHECK_EQUAL(c, (Card{Suit::hearts, SuitRank::king}));
  }
  {
    Card c;
    BOOST_CHECK_EQUAL(true, Card::from_string(c, "hk"));
    BOOST_CHECK_EQUAL(c, (Card{Suit::hearts, SuitRank::king}));
  }
}

BOOST_AUTO_TEST_CASE(card_to_string)
{
  {
    Card c{TrumpRank::skus};
    BOOST_CHECK_EQUAL("Sk", boost::lexical_cast<std::string>(c));
  }
  {
    Card c{Suit::hearts, SuitRank::king};
    BOOST_CHECK_EQUAL("HK", boost::lexical_cast<std::string>(c));
  }
  {
    Card c{Suit::clubs, SuitRank::ten};
    BOOST_CHECK_EQUAL("Ct", boost::lexical_cast<std::string>(c));
  }
  {
    Card c{Suit::diamonds, SuitRank::ace};
    BOOST_CHECK_EQUAL("D1", boost::lexical_cast<std::string>(c));
  }
}

}

