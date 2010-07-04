#ifndef KONIG__PLAYPOSITION_HPP
#define KONIG__PLAYPOSITION_HPP

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

inline PlayPosition& operator%=(PlayPosition& p, int v) {
  assert(v==4);
  p = PlayPosition(p%4);
  return p;
}

}

#endif // KONIG__PLAYPOSITION_HPP

