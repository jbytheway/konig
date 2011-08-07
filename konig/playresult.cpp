#include <konig/playresult.hpp>

namespace konig {

void PlayResult::dump(std::ostream& o) const
{
  o << outcome << '\n';
  o << '\n';
  std::copy(
    tricks.begin(), tricks.end(),
    std::ostream_iterator<konig::Trick>(o, "\n")
  );
  o << '\n';
  std::copy(
    scores.begin(), scores.end(),
    std::ostream_iterator<Score>(o, " ")
  );
  o << '\n';
}

}

