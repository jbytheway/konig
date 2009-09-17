#include "gametracker.hpp"

#include <konig/fatal.hpp>

namespace konig { namespace dterm {

int GameTracker::bid()
{
  KONIG_FATAL("not implemented");
}

KingCall GameTracker::call_king()
{
  KONIG_FATAL("not implemented");
}

uint8_t GameTracker::choose_talon_half()
{
  KONIG_FATAL("not implemented");
}

Cards GameTracker::discard()
{
  KONIG_FATAL("not implemented");
}

std::vector<Announcement> GameTracker::announce()
{
  KONIG_FATAL("not implemented");
}

Card GameTracker::play_card()
{
  KONIG_FATAL("not implemented");
}

}}

