#ifndef KONIG_TERMINAL_UIMODE_HPP
#define KONIG_TERMINAL_UIMODE_HPP

namespace konig { namespace terminal {

class UiMode {
  public:
    virtual ~UiMode() = 0;
};

inline UiMode::~UiMode() = default;

}}

#endif // KONIG_TERMINAL_UIMODE_HPP

