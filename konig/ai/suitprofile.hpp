#ifndef KONIG_AI_SUITPROFILE_HPP
#define KONIG_AI_SUITPROFILE_HPP

#include <vector>

#include <konig/cards.hpp>

namespace konig { namespace ai {

struct SuitProfile {
  /// Makes a list of profiles, sorted (not stably) by size and then points
  static std::vector<SuitProfile> make_profiles(Cards const&);

  size_t size;
  size_t points;
  Suit suit;

  friend bool operator<(SuitProfile const& l, SuitProfile const& r) {
    return std::make_tuple(l.size, l.points) <
      std::make_tuple(r.size, r.points);
  }
};

}}

#endif // KONIG_AI_SUITPROFILE_HPP

