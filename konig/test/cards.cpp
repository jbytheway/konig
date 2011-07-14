#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <konig/cards.hpp>

namespace konig {

BOOST_AUTO_TEST_CASE(cards_parser)
{
  //BOOST_CHECK_EQUAL(Cards::from_string(""), Cards());
  BOOST_CHECK_EQUAL(Cards::from_string("Sk"), Cards{Card(TrumpRank::skus)});
  BOOST_CHECK_EQUAL(Cards::from_string("21"), Cards{Card(TrumpRank::mond)});
  BOOST_CHECK_EQUAL(Cards::from_string("1"), Cards{Card(TrumpRank::pagat)});
  BOOST_CHECK_EQUAL(
    Cards::from_string("H:K"), (Cards{{Suit::hearts, SuitRank::king}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("HK"), (Cards{{Suit::hearts, SuitRank::king}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("C:t"), (Cards{{Suit::clubs, SuitRank::ten}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("Ct"), (Cards{{Suit::clubs, SuitRank::ten}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("D:3"), (Cards{{Suit::diamonds, SuitRank::three}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("Sk 21"),
    (Cards{Card{TrumpRank::skus}, Card{TrumpRank::mond}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("Sk 21  "),
    (Cards{Card{TrumpRank::skus}, Card{TrumpRank::mond}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("Sk_21"),
    (Cards{Card{TrumpRank::skus}, Card{TrumpRank::mond}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("Sk_ 21 _"),
    (Cards{Card{TrumpRank::skus}, Card{TrumpRank::mond}})
  );
}

}

