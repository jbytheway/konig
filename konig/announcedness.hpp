#ifndef KONIG__ANNOUNCEDNESS_HPP
#define KONIG__ANNOUNCEDNESS_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include <konig/achievement.hpp>

namespace konig {

class Announcedness {
  public:
    enum internal_enum {
      unannounced,
      announced,
      kontraed,
      rekontraed,
      subkontraed
    };

    Announcedness() : value_(unannounced) {}

    Announcedness(const internal_enum v) : value_(v) {}

    operator internal_enum() const { return value_; }

    // Returns true iff this level of announcedness is announced by the side
    // that didn't originally announce the feat
    bool inverted_side() const {
      return value_ == kontraed || value_ == subkontraed;
    }

    Announcedness next_level() const {
      return Announcedness(internal_enum(value_ + 1));
    }

    std::string string(const Achievement) const;
  private:
    internal_enum value_;
};

}

#endif // KONIG__ANNOUNCEDNESS_HPP

