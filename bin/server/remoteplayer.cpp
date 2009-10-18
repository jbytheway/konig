#include "remoteplayer.hpp"

#include <konig/fatal.hpp>

namespace konig { namespace server {

RemotePlayer::RemotePlayer(Client& client) :
  client_(client)
{
}

void RemotePlayer::start_game(Ruleset rules, PlayPosition pos, Cards cards)
{
  client_.send(Message<MessageType::startGame>(
        std::move(rules), std::move(pos), std::move(cards)
      ));
}

int RemotePlayer::bid()
{
  KONIG_FATAL("not implemented");
}

void RemotePlayer::notify_bid(PlayPosition, int)
{
  KONIG_FATAL("not implemented");
}

KingCall RemotePlayer::call_king()
{
  KONIG_FATAL("not implemented");
}

void RemotePlayer::notify_call_king(KingCall)
{
  KONIG_FATAL("not implemented");
}

void RemotePlayer::notify_talon(boost::array<Cards, 2> const& /*talon*/)
{
  KONIG_FATAL("not implemented");
}

uint8_t RemotePlayer::choose_talon_half()
{
  KONIG_FATAL("not implemented");
}

void RemotePlayer::notify_talon_choice(uint8_t)
{
  KONIG_FATAL("not implemented");
}

Cards RemotePlayer::discard()
{
  KONIG_FATAL("not implemented");
}

void RemotePlayer::notify_discard(Cards)
{
  KONIG_FATAL("not implemented");
}

std::vector<Announcement> RemotePlayer::announce()
{
  KONIG_FATAL("not implemented");
}

void RemotePlayer::notify_announcements(std::vector<Announcement>)
{
  KONIG_FATAL("not implemented");
}

Card RemotePlayer::play_card()
{
  KONIG_FATAL("not implemented");
}

void RemotePlayer::notify_play_card(PlayPosition, Card)
{
  KONIG_FATAL("not implemented");
}

}}

