#include <konig/ai/suitprofile.hpp>

#include <map>

#include <boost/foreach.hpp>

namespace konig { namespace ai {

std::vector<SuitProfile> SuitProfile::make_profiles(Cards const& cards)
{
  // collect size and value of each suit
  std::map<Suit, std::pair<size_t, size_t>> suits;
  BOOST_FOREACH(const Card& card, cards) {
    std::map<Suit, std::pair<size_t, size_t>>::iterator i =
      suits.insert(std::make_pair(card.suit(), std::make_pair(0, 0))).first;
    i->second.first += 1;
    i->second.second += card.card_points();
  }
  std::vector<SuitProfile> result;
  // Invert the map so we know what our shortest suits are
  BOOST_FOREACH(const auto& s, suits) {
    result.push_back(
        SuitProfile{s.second.first, s.second.second, s.first}
      );
  }
  std::sort(result.begin(), result.end());
  return result;
}

}}

