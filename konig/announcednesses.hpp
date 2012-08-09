#ifndef KONIG_ANNOUNCEDNESSES_HPP
#define KONIG_ANNOUNCEDNESSES_HPP

#include <map>

#include <boost/container/flat_map.hpp>

#include <konig/announcedness.hpp>

namespace konig {

//typedef std::map<std::pair<Feat, bool>, Announcedness> AnnouncednessesImpl;
typedef boost::container::flat_map<
  std::pair<Feat, bool>, Announcedness
> AnnouncednessesImpl;

class Announcednesses : public AnnouncednessesImpl {
  public:
    Announcednesses() = default;

    using AnnouncednessesImpl::insert;

    std::pair<iterator, bool> insert(
        Feat f,
        bool offence,
        Announcedness a = Announcedness::unannounced
      ) {
      return insert(std::make_pair(std::make_pair(f, offence), a));
    }

    iterator insert(
      const_iterator i,
      Feat f,
      bool offence,
      Announcedness a = Announcedness::unannounced
    ) {
      return insert(i, std::make_pair(std::make_pair(f, offence), a));
    }
};

}

#endif // KONIG_ANNOUNCEDNESSES_HPP

