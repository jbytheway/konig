#ifndef KONIG_REAL_REALGAME_HPP
#define KONIG_REAL_REALGAME_HPP

#include <konig/ruleset.hpp>
#include <konig/oracle.hpp>

namespace konig { namespace real {

class RealGame {
  public:
    RealGame(Ruleset const&, Oracle&);

    PlayResult play();
  private:
    Ruleset const& rules_;
    Oracle& oracle_;
};

}}

#endif // KONIG_REAL_REALGAME_HPP

