#ifndef KONIG_TERMINAL_UIMODES_HPP
#define KONIG_TERMINAL_UIMODES_HPP

#include <konig/terminal/uimode.hpp>
#include <konig/terminal/checkers.hpp>

namespace konig { namespace terminal {

namespace uiModes {

  struct Bid : UiMode {
    typedef checkers::BidChecker pre_checker_type;
    static char const* prompt() { return "bid> "; }
  };

  struct CallKing : UiMode {
    typedef checkers::KingChecker pre_checker_type;
    static char const* prompt() { return "call> "; }
  };

  struct ChooseConcede : UiMode {
    typedef checkers::BoolChecker pre_checker_type;
    static char const* prompt() { return "concede?> "; }
  };

  struct ChooseTalonHalf : UiMode {
    typedef checkers::TalonChecker pre_checker_type;
    static char const* prompt() { return "choose> "; }
  };

  struct Discard : UiMode {
    typedef checkers::CardsChecker pre_checker_type;
    static char const* prompt() { return "discard> "; }
  };

  struct Announce : UiMode {
    typedef checkers::AnnounceChecker pre_checker_type;
    static char const* prompt() { return "announce> "; }
  };

  struct PlayCard : UiMode {
    typedef checkers::CardChecker pre_checker_type;
    static char const* prompt() { return "play> "; }
  };

}

}}

#endif // KONIG_TERMINAL_UIMODES_HPP

