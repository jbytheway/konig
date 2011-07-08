#include <konig/announcement.hpp>

namespace konig {

Announcement::Announcement(Feat f, Announcedness a, bool d) :
  feat(f),
  announcedness(a),
  defensive(d)
{}

bool Announcement::from_range(
  Announcement& a,
  std::string::const_iterator& start,
  std::string::const_iterator const& finish
)
{
  if (start == finish) return false;
  switch (*start) {
    case '1':
      a.feat = Feat::pagat;
      ++start;
      break;
    case '2':
      a.feat = Feat::uhu;
      ++start;
      break;
    case '3':
      a.feat = Feat::kakadu;
      ++start;
      break;
    case 'f':
      a.feat = Feat::forty_five;
      ++start;
      break;
    case 'v':
      a.feat = Feat::valat;
      ++start;
      break;
    case 'k':
      a.feat = Feat::king_ultimo;
      ++start;
      break;
    case 'x':
      a.feat = Feat::game;
      // Do *not* increment start
      break;
    default:
      return false;
  }
  a.announcedness = Announcedness::announced;
  if (start == finish) {
    a.defensive = false;
    return true;
  }

  if (*start == 'd' || *start == '!') {
    a.defensive = true;
    ++start;
  } else {
    a.defensive = false;
  }

  while (start != finish && *start == 'x' &&
      a.announcedness != Announcedness::subkontraed) {
    ++start;
    a.announcedness = a.announcedness.next_level();
  }
  return true;
}

bool Announcement::from_string(Announcement& a, std::string const& s)
{
  auto at = s.begin();
  if (!from_range(a, at, s.end())) return false;
  return at == s.end();
}

bool Announcement::many_from_string(
  std::vector<Announcement>& v, std::string const& s
)
{
  auto at = s.begin();
  Announcement a;
  while (from_range(a, at, s.end())) {
    v.push_back(a);
  }
  return at == s.end();
}

std::ostream& operator<<(std::ostream& o, Announcement const& a)
{
  o << a.feat;
  if (a.defensive) o << '!';
  o << a.announcedness.string(Achievement::neutral);
  return o;
}

}

