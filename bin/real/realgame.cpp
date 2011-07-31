#include "realgame.hpp"

namespace konig { namespace real {

RealGame::RealGame(Ruleset const& rules, Oracle& oracle) :
  rules_(rules),
  oracle_(oracle)
{
}

void RealGame::play()
{
  KONIG_FATAL("not implemented");
}

}}

