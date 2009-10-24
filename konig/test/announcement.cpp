#include <konig/announcement.hpp>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace konig;

BOOST_AUTO_TEST_CASE(Announcement_parses_correctly)
{
  Announcement a;
  BOOST_CHECK(!Announcement::from_string(a, ""));
  BOOST_CHECK(Announcement::from_string(a, "x"));
  BOOST_CHECK(a == Announcement(Feat::game, Announcedness::kontraed, false));
  BOOST_CHECK(Announcement::from_string(a, "f!"));
  BOOST_CHECK(
      a == Announcement(Feat::forty_five, Announcedness::announced, true)
    );
  BOOST_CHECK(Announcement::from_string(a, "fdxx"));
  BOOST_CHECK(
      a == Announcement(Feat::forty_five, Announcedness::rekontraed, true)
    );
}

