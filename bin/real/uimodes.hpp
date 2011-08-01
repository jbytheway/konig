#ifndef KONIG_REAL_UIMODES_HPP
#define KONIG_REAL_UIMODES_HPP

#include "checkers.hpp"

namespace konig { namespace real {

namespace uiModes {

  struct PlayPosition : terminal::UiMode {
    typedef checkers::PlayPositionChecker pre_checker_type;
    static inline char const* prompt() { return "position> "; }
  };

}

}}

#endif // KONIG_REAL_UIMODES_HPP

