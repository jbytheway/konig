#ifndef KONIG_DTERM__UIMODE_HPP
#define KONIG_DTERM__UIMODE_HPP

namespace konig { namespace dterm {

enum class UiMode {
  none,
  bid,
  callKing,
  chooseConcession,
  chooseTalonHalf,
  discard,
  announce,
  playCard,
  max // Must be last
};

}}

#endif // KONIG_DTERM__UIMODE_HPP

