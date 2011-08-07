#ifndef KONIG_REAL_REALGAME_HPP
#define KONIG_REAL_REALGAME_HPP

#include <konig/ruleset.hpp>

#include "oneaioracle.hpp"

namespace konig { namespace real {

class RealGame {
  public:
    RealGame(Ruleset const&, OneAiOracle&);

    PlayResult play();
  private:
    Ruleset const& rules_;
    OneAiOracle& oracle_;
};

}}

#endif // KONIG_REAL_REALGAME_HPP

