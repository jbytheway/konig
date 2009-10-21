#include <konig/kingcall.hpp>

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

}

