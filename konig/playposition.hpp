#ifndef KONIG_PLAYPOSITION_HPP
#define KONIG_PLAYPOSITION_HPP

#include <konig/core.hpp>

namespace konig {

enum PlayPosition {
  position_forehand = 0,
  position_max = 4
};

inline PlayPosition& operator++(PlayPosition& p) {
  // Don't want to reduce mod 4, becasue then couldn't easily loop over all
  // PlayPositions
  return p = PlayPosition(p+1);
}

template<typename T>
inline typename boost::enable_if<std::is_integral<T>, PlayPosition>::type
operator+(PlayPosition const p, T const v) {
  // Don't want to reduce mod 4, becasue then couldn't easily loop over all
  // PlayPositions
  return PlayPosition(int(p)+v);
}

inline PlayPosition operator%(PlayPosition const p, int const v) {
  assert(v==4);
  return PlayPosition(int(p)%4);
}

inline PlayPosition& operator%=(PlayPosition& p, int const v) {
  assert(v==4);
  p = PlayPosition(p%4);
  return p;
}

}

#endif // KONIG_PLAYPOSITION_HPP

