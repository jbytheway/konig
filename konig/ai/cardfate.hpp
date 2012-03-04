#ifndef KONIG_AI_CARDFATE_HPP
#define KONIG_AI_CARDFATE_HPP

#include <konig/utility/enum.hpp>

namespace konig { namespace ai {

class CardFate {
  public:
    enum internal_enum {
      hand0,
      hand1,
      hand2,
      hand3,
      played0,
      played1,
      played2,
      played3,
      talon,
      reject,
      discard,
      max // Must be last
    };

    static internal_enum held_by(PlayPosition const p) {
      return internal_enum(p);
    }

    static internal_enum played_by(PlayPosition const p) {
      return internal_enum(p+played0);
    }

    CardFate(internal_enum const v) : value_(v) {}

    friend inline bool operator==(CardFate const l, CardFate const r) {
      return l.value_ == r.value_;
    }
    friend inline bool operator<(CardFate const l, CardFate const r) {
      return l.value_ < r.value_;
    }
  private:
    internal_enum value_;
};

}}

#endif // KONIG_AI_CARDFATE_HPP

