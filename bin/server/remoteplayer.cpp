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

#define KONIG_SERVER_REMOTE_PLAYER_REMOTE_CALL(name, request, response)      \
Message<MessageType::response>::only_value RemotePlayer::name()              \
{                                                                            \
  return client_.remote_call<MessageType::request, MessageType::response>(); \
}
KONIG_SERVER_REMOTE_PLAYER_REMOTE_CALL(bid, requestBid, bid)
KONIG_SERVER_REMOTE_PLAYER_REMOTE_CALL(call_king, requestCallKing, callKing)
KONIG_SERVER_REMOTE_PLAYER_REMOTE_CALL(
    choose_talon_half, requestTalonChoice, talonChoice
  )
KONIG_SERVER_REMOTE_PLAYER_REMOTE_CALL(discard, requestDiscard, discard)
KONIG_SERVER_REMOTE_PLAYER_REMOTE_CALL(
    announce, requestAnnouncements, announcements
  )
KONIG_SERVER_REMOTE_PLAYER_REMOTE_CALL(play_card, requestPlayCard, playCard)
#undef KONIG_SERVER_REMOTE_PLAYER_REMOTE_CALL

void RemotePlayer::notify_bid(PlayPosition pos, Bid bid)
{
  client_.send(Message<MessageType::notifyBid>(
        std::move(pos), std::move(bid)
      ));
}

void RemotePlayer::notify_contract_established(Bid bid)
{
  client_.send(Message<MessageType::notifyContractEstablished>(
        std::move(bid)
      ));
}

void RemotePlayer::notify_call_king(KingCall call)
{
  client_.send(Message<MessageType::notifyCallKing>(
        std::move(call)
      ));
}

void RemotePlayer::notify_talon(std::array<Cards, 2> const& talon)
{
  client_.send(Message<MessageType::notifyTalon>(
        std::move(talon)
      ));
}

void RemotePlayer::notify_talon_choice(uint8_t choice)
{
  client_.send(Message<MessageType::notifyTalonChoice>(
        std::move(choice)
      ));
}

void RemotePlayer::notify_discard(Cards discard)
{
  client_.send(Message<MessageType::notifyDiscard>(
        std::move(discard)
      ));
}

void RemotePlayer::notify_announcements(std::vector<Announcement> announcements)
{
  client_.send(Message<MessageType::notifyAnnouncements>(
        std::move(announcements)
      ));
}

void RemotePlayer::notify_invalid_announcements(std::string m)
{
  client_.send(Message<MessageType::notifyInvalidAnnouncements>(std::move(m)));
}

void RemotePlayer::notify_announcements_done()
{
  client_.send(Message<MessageType::notifyAnnouncementsDone>());
}

void RemotePlayer::notify_play_card(PlayPosition pos, Card card)
{
  client_.send(Message<MessageType::notifyPlayCard>(
        std::move(pos), std::move(card)
      ));
}

void RemotePlayer::notify_invalid_play(std::string m)
{
  client_.send(Message<MessageType::notifyInvalidPlay>(std::move(m)));
}

}}

