#ifndef KONIG_REAL_REALGAME_HPP
#define KONIG_REAL_REALGAME_HPP

#include <konig/ruleset.hpp>

namespace konig { namespace real {

class Oracle;

class RealGame {
  public:
    RealGame(Ruleset const&, Oracle&);

    void play();
  private:
    Ruleset const& rules_;
    Oracle& oracle_;
};

}}

#endif // KONIG_REAL_REALGAME_HPP

