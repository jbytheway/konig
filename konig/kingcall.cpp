#include <konig/kingcall.hpp>

#include <konig/fatal.hpp>

namespace konig {

bool KingCall::from_string(KingCall& call, std::string const& s)
{
  if (s.size() != 1) return false;
  switch (s[0]) {
    case 'c':
    case 'C':
      call = clubs;
      return true;
    case 'd':
    case 'D':
      call = diamonds;
      return true;
    case 'h':
    case 'H':
      call = hearts;
      return true;
    case 's':
    case 'S':
      call = spades;
      return true;
    case '4':
      call = fourth_king;
      return true;
  }
  return false;
}

std::string KingCall::string() const
{
  switch (value_) {
    case clubs:
      return "C";
    case diamonds:
      return "D";
    case hearts:
      return "H";
    case spades:
      return "S";
    case fourth_king:
      return "4";
    default:
      KONIG_FATAL("invalid KingCall");
  }
}

std::ostream& operator<<(std::ostream& o, KingCall const c)
{
  return o << c.string();
}

}

