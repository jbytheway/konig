#ifndef KONIG_PLAYRESULT_HPP
#define KONIG_PLAYRESULT_HPP

#include <konig/score.hpp>

namespace konig {

struct PlayResult {
  Outcome outcome;
  std::vector<Trick> tricks;
  std::array<Score, 4> scores;
};

}

#endif // KONIG_PLAYRESULT_HPP

