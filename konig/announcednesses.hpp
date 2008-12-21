#ifndef KONIG__ANNOUNCEDNESSES_HPP
#define KONIG__ANNOUNCEDNESSES_HPP

#include <map>

#include <konig/core.hpp>

namespace konig {

class Announcednesses : public std::map<std::pair<bool, Feat>, Announcedness> {
  public:
    using std::map<std::pair<bool, Feat>, Announcedness>::insert;

    std::pair<iterator, bool> insert(
        bool offence,
        Feat f,
        Announcedness a = Announcedness::unannounced
      ) {
      return insert(std::make_pair(std::make_pair(offence, f), a));
    }
};

}

#endif // KONIG__ANNOUNCEDNESSES_HPP

