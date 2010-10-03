#include <array>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <konig/deal.hpp>

namespace konig {

BOOST_AUTO_TEST_CASE(deal_constructors_work)
{
  BOOST_TEST_CHECKPOINT("deal one-argument constructor");
  std::vector<std::string> chunk_strings{
      "21 12 10 8 6 C:98 D:Q8 H:Kt S:J",
      "Sk 16 14 C:KQ D:NJ H:97 S:t97",
      "20 19 13 9 5 3 1 C:J7 D:t7 S:8",
      "18 17 15 11 C:Nt D:K H:QN S:KQN",
      "7 4 H:8",
      "2 D:9 H:J"
  };
  {
    std::array<Cards, 6> chunks;
    for (size_t i=0; i<6; ++i) {
      chunks[i] = Cards::from_string(chunk_strings[i]);
    }
    Deal deal(chunks);
  }
  {
    BOOST_TEST_CHECKPOINT("deal two-argument constructor");
    std::array<Cards, 4> hands;
    std::array<Cards, 2> talon;
    for (size_t i=0; i<4; ++i) {
      hands[i] = Cards::from_string(chunk_strings[i]);
    }
    for (size_t i=0; i<2; ++i) {
      talon[i] = Cards::from_string(chunk_strings[i+4]);
    }
    Deal deal(hands, talon);
  }
}

}

