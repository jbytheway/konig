#ifndef KONIG__ANNOUNCEDNESSES_HPP
#define KONIG__ANNOUNCEDNESSES_HPP

#include <map>

#include <konig/announcedness.hpp>

namespace konig {

class Announcednesses : public std::map<std::pair<Feat, bool>, Announcedness> {
  public:
    using std::map<std::pair<Feat, bool>, Announcedness>::insert;

    std::pair<iterator, bool> insert(
        Feat f,
        bool offence,
        Announcedness a = Announcedness::unannounced
      ) {
      return insert(std::make_pair(std::make_pair(f, offence), a));
    }
};

}

#endif // KONIG__ANNOUNCEDNESSES_HPP

