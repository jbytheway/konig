#ifndef KONIG__ANNOUNCEMENT_HPP
#define KONIG__ANNOUNCEMENT_HPP

#include <konig/feat.hpp>
#include <konig/announcedness.hpp>

namespace konig {

struct Announcement {
  Feat feat;
  Announcedness announcedness;
  bool defensive;

  bool offensive() const { return !defensive; }
};

}

#endif // KONIG__ANNOUNCEMENT_HPP

