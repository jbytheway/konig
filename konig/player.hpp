#ifndef KONIG__PLAYER_HPP
#define KONIG__PLAYER_HPP

#include <boost/shared_ptr.hpp>

#include <konig/api.hpp>
#include <konig/bid.hpp>
#include <konig/ruleset.hpp>
#include <konig/cards.hpp>
#include <konig/playposition.hpp>
#include <konig/kingcall.hpp>
#include <konig/announcement.hpp>

namespace konig {

class KONIG_API Player {
  public:
    typedef boost::shared_ptr<Player> Ptr;

    virtual void start_game(Ruleset, PlayPosition, Cards) = 0;
    virtual Bid bid() = 0;
    virtual void notify_bid(PlayPosition, Bid) = 0;
    virtual void notify_contract_established(Bid bid) = 0;
    virtual KingCall call_king() = 0;
    virtual void notify_call_king(KingCall) = 0;
    virtual void notify_talon(std::array<Cards, 2> const& talon) = 0;
    virtual bool choose_concede() = 0;
    virtual void notify_concede() = 0;
    virtual uint8_t choose_talon_half() = 0;
    virtual void notify_talon_choice(uint8_t) = 0;
    virtual Cards discard() = 0;
    virtual void notify_discard(Cards) = 0;
    virtual std::vector<Announcement> announce() = 0;
    virtual void notify_announcements(std::vector<Announcement>) = 0;
    virtual void notify_announcements_done() {}
    virtual Card play_card() = 0;
    virtual void notify_play_card(PlayPosition, Card) = 0;
    virtual void notify_ouvert(Cards const&) = 0;
    virtual void notify_invalid(std::string) = 0;
  protected:
    Player() {};
    Player(Player const&);
    Player& operator=(Player const&);
    virtual ~Player() = 0;
};

inline Player::Player(Player const&) = default;
inline Player& Player::operator=(Player const&) = default;

}

#endif // KONIG__PLAYER_HPP

