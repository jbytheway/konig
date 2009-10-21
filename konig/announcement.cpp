#include <konig/announcement.hpp>

namespace konig {

bool Announcement::from_string(Announcement& a, std::string const& d)
{
  if (d.size() == 0) return false;
  size_t p = d.size();
  a.announcedness = Announcedness::unannounced;
  while (p != 0 && d[p-1] == 'x' &&
      a.announcedness != Announcedness::subkontraed) {
    --p;
    a.announcedness = a.announcedness.next_level();
  }
  if (p > 1) return false;
  if (p == 0) {
    a.feat = Feat::game;
    return true;
  }
  switch (d[0]) {
    case '1':
      a.feat = Feat::pagat;
      return true;
    case '2':
      a.feat = Feat::uhu;
      return true;
    case '3':
      a.feat = Feat::kakadu;
      return true;
    case 'f':
      a.feat = Feat::forty_five;
      return true;
    case 'v':
      a.feat = Feat::valat;
      return true;
    case 'k':
      a.feat = Feat::king_ultimo;
      return true;
  }
  return false;
}

}

