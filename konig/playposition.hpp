#ifndef KONIG__PLAYPOSITION_HPP
#define KONIG__PLAYPOSITION_HPP

#include <konig/core.hpp>

namespace konig {

enum PlayPosition {
  position_forehand = 0,
  position_max = 4
};

inline PlayPosition& operator++(PlayPosition& p) {
  return p = PlayPosition(p+1);
}

}

#endif // KONIG__PLAYPOSITION_HPP

