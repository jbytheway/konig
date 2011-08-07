#ifndef KONIG_PLAYRESULT_HPP
#define KONIG_PLAYRESULT_HPP

#include <konig/outcome.hpp>
#include <konig/score.hpp>

namespace konig {

struct PlayResult {
  Outcome outcome;
  std::vector<Trick> tricks;
  std::array<Score, 4> scores;

  void dump(std::ostream&) const;
};

}

#endif // KONIG_PLAYRESULT_HPP

