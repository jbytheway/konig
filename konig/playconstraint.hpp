#ifndef KONIG__PLAYCONSTRAINT_HPP
#define KONIG__PLAYCONSTRAINT_HPP

namespace konig {

class PlayConstraint {
  public:
    enum internal_enum {
      neutral,
      avoid_playing,
      try_to_play
    };

    PlayConstraint(internal_enum v) : value_(v) {}
    operator internal_enum() const { return value_; }
  private:
    internal_enum value_;
};

}

#endif // KONIG__PLAYCONSTRAINT_HPP

