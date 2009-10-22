#include "gametracker.hpp"

#include <konig/fatal.hpp>

#include "commandhandler.hpp"

namespace konig { namespace dterm {

GameTracker::GameTracker(CommandHandler& handler) :
  handler_(handler)
{}

int GameTracker::bid()
{
  handler_.present_bidding();
  return handler_.get_from_user<int>(UiMode::bid);
}

KingCall GameTracker::call_king()
{
  return handler_.get_from_user<KingCall>(UiMode::callKing);
}

uint8_t GameTracker::choose_talon_half()
{
  return handler_.get_from_user<uint8_t>(UiMode::chooseTalonHalf);
}

Cards GameTracker::discard()
{
  return handler_.get_from_user<Cards>(UiMode::discard);
}

std::vector<Announcement> GameTracker::announce()
{
  return handler_.get_from_user<std::vector<Announcement>>(UiMode::announce);
}

Card GameTracker::play_card()
{
  return handler_.get_from_user<Card>(UiMode::playCard);
}

}}

