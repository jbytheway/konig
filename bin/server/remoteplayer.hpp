#ifndef KONIG_SERVER__REMOTE_PLAYER_HPP
#define KONIG_SERVER__REMOTE_PLAYER_HPP

#include <konig/player.hpp>

#include "client.hpp"

namespace konig { namespace server {

class RemotePlayer : public Player {
  public:
    RemotePlayer(Client&);

    virtual void start_game(Ruleset, PlayPosition, Cards);
    virtual Bid bid();
    virtual void notify_bid(PlayPosition, Bid);
    virtual KingCall call_king();
    virtual void notify_call_king(KingCall);
    virtual void notify_talon(std::array<Cards, 2> const& talon);
    virtual uint8_t choose_talon_half();
    virtual void notify_talon_choice(uint8_t);
    virtual Cards discard();
    virtual void notify_discard(Cards);
    virtual std::vector<Announcement> announce();
    virtual void notify_announcements(std::vector<Announcement>);
    virtual void notify_announcements_done();
    virtual Card play_card();
    virtual void notify_play_card(PlayPosition, Card);
    virtual void notify_invalid_play(std::string);
  private:
    Client& client_;
};

}}

#endif // KONIG_SERVER__REMOTE_PLAYER_HPP

