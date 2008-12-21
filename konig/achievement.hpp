#ifndef KONIG__ACHIEVEMENT_HPP
#define KONIG__ACHIEVEMENT_HPP

#include <konig/core.hpp>

namespace konig {

class Achievement {
  public:
    enum internal_enum {
      off,
      neutral,
      made
    };

    Achievement() : value_(neutral) {}
    Achievement(const internal_enum v) : value_(v) {}

    operator internal_enum() const { return value_; }
  private:
    internal_enum value_;
};

}

#endif // KONIG__ACHIEVEMENT_HPP

