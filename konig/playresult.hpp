#ifndef KONIG__PLAYRESULT_HPP
#define KONIG__PLAYRESULT_HPP

namespace konig {

struct PlayResult {
  Outcome outcome;
  std::vector<Trick> tricks;
  std::array<int, 4> scores;
};

}

#endif // KONIG__PLAYRESULT_HPP

