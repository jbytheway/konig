#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/lexical_cast.hpp>

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
    Cards::from_string("h:k"), (Cards{{Suit::hearts, SuitRank::king}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("HK"), (Cards{{Suit::hearts, SuitRank::king}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("hk"), (Cards{{Suit::hearts, SuitRank::king}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("H:KQ"),
    (Cards{{Suit::hearts, SuitRank::king}, {Suit::hearts, SuitRank::queen}})
  );
  BOOST_CHECK_EQUAL(
    Cards::from_string("HKq"),
    (Cards{{Suit::hearts, SuitRank::king}, {Suit::hearts, SuitRank::queen}})
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

BOOST_AUTO_TEST_CASE(cards_to_string)
{
  {
    Cards c{Card{TrumpRank::skus}};
    BOOST_CHECK_EQUAL("Sk ", boost::lexical_cast<std::string>(c));
  }
  {
    Cards c{Card{TrumpRank::skus}, Card{TrumpRank::mond}};
    BOOST_CHECK_EQUAL("Sk 21 ", boost::lexical_cast<std::string>(c));
  }
  {
    Cards c{{Suit::hearts, SuitRank::king}};
    BOOST_CHECK_EQUAL("H:K ", boost::lexical_cast<std::string>(c));
  }
  {
    Cards c{{Suit::hearts, SuitRank::king}, {Suit::hearts, SuitRank::queen}};
    BOOST_CHECK_EQUAL("H:KQ ", boost::lexical_cast<std::string>(c));
  }
  {
    Cards c{{Suit::spades, SuitRank::king}, {Suit::hearts, SuitRank::queen}};
    BOOST_CHECK_EQUAL("H:Q S:K ", boost::lexical_cast<std::string>(c));
  }
  {
    Cards c{{Suit::clubs, SuitRank::ten}};
    BOOST_CHECK_EQUAL("C:t ", boost::lexical_cast<std::string>(c));
  }
  {
    Cards c{{Suit::diamonds, SuitRank::ace}};
    BOOST_CHECK_EQUAL("D:1 ", boost::lexical_cast<std::string>(c));
  }
}

}

