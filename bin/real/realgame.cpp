#include "realgame.hpp"

#include "oracle.hpp"

namespace konig { namespace real {

RealGame::RealGame(Ruleset const& rules, Oracle& oracle) :
  rules_(rules),
  oracle_(oracle)
{
}

void RealGame::play()
{
  oracle_.init();
}

}}

