#include "gametracker.hpp"

#include <konig/fatal.hpp>

#include "commandhandler.hpp"

namespace konig { namespace dterm {

GameTracker::GameTracker(CommandHandler& handler) :
  handler_(handler)
{}

void GameTracker::notify_talon(const std::array<Cards, 2>& talon)
{
  Ai::notify_talon(talon);
  handler_.present_talon();
}

void GameTracker::notify_invalid(std::string m)
{
  handler_.message(std::move(m));
}

void GameTracker::game_start_hook()
{
  handler_.present_hand();
}

void GameTracker::trick_complete_hook()
{
  handler_.present_current_trick();
}

Bid GameTracker::bid()
{
  handler_.present_bidding();
  return handler_.get_from_user<Bid>(UiMode::bid);
}

KingCall GameTracker::call_king()
{
  return handler_.get_from_user<KingCall>(UiMode::callKing);
}

bool GameTracker::choose_concede()
{
  return handler_.get_from_user<bool>(UiMode::chooseConcession);
}

uint8_t GameTracker::choose_talon_half()
{
  return handler_.get_from_user<uint8_t>(UiMode::chooseTalonHalf);
}

Cards GameTracker::discard()
{
  handler_.present_hand(true);
  return handler_.get_from_user<Cards>(UiMode::discard);
}

std::vector<Announcement> GameTracker::announce()
{
  handler_.present_contract();
  return handler_.get_from_user<std::vector<Announcement>>(UiMode::announce);
}

void GameTracker::notify_announcements_done()
{
  handler_.present_contract();
}

Card GameTracker::play_card()
{
  handler_.present_hand();
  handler_.present_current_trick();
  return handler_.get_from_user<Card>(UiMode::playCard);
}

}}

