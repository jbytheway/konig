#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <konig/cards.hpp>

namespace konig {

BOOST_AUTO_TEST_CASE(cards_parser)
{
  BOOST_CHECK_EQUAL(Cards::from_string(""), Cards());
  BOOST_CHECK_EQUAL(Cards::from_string("Sk"), Cards{Card(TrumpRank::skus)});
}

}

