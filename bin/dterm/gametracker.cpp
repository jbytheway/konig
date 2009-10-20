#include "gametracker.hpp"

#include <konig/fatal.hpp>

#include "commandhandler.hpp"

namespace konig { namespace dterm {

GameTracker::GameTracker(CommandHandler& handler) :
  handler_(handler)
{}

int GameTracker::bid()
{
  return handler_.get_from_user<int>(UiMode::bid);
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

