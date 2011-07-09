#ifndef KONIG__PLAYRESULT_HPP
#define KONIG__PLAYRESULT_HPP

#include <konig/score.hpp>

namespace konig {

struct PlayResult {
  Outcome outcome;
  std::vector<Trick> tricks;
  std::array<Score, 4> scores;
};

}

#endif // KONIG__PLAYRESULT_HPP

