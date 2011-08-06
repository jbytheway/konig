#ifndef KONIG_REAL_UIMODES_HPP
#define KONIG_REAL_UIMODES_HPP

#include "checkers.hpp"

namespace konig { namespace real {

namespace uiModes {

  struct PlayPosition : terminal::UiMode {
    typedef checkers::PlayPositionChecker pre_checker_type;
    static inline char const* prompt() { return "position> "; }
  };

  struct Hand : terminal::UiMode {
    typedef terminal::checkers::CardsChecker pre_checker_type;
    static inline char const* prompt() { return "hand> "; }
  };

  struct Talon : terminal::UiMode {
    typedef terminal::checkers::CardsChecker pre_checker_type;
    static inline char const* prompt() { return "talon half> "; }
  };

  struct Ouvert : terminal::UiMode {
    typedef terminal::checkers::CardsChecker pre_checker_type;
    static inline char const* prompt() { return "ouvert> "; }
  };

}

}}

#endif // KONIG_REAL_UIMODES_HPP

