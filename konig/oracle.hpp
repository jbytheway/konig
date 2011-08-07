#ifndef KONIG_ORACLE_HPP
#define KONIG_ORACLE_HPP

#include <konig/bid.hpp>
#include <konig/cards.hpp>
#include <konig/playposition.hpp>
#include <konig/announcement.hpp>

namespace konig {

class Ruleset;

/** An Oracle provides information about a game where we don't have total
 * knowledge from the get go. */
class Oracle {
  public:
    virtual ~Oracle() = 0;

    virtual Bid bid(PlayPosition) = 0;
    virtual void notify_bid(PlayPosition, Bid) = 0;
    virtual void notify_contract_established(Bid bid) = 0;
    virtual KingCall call_king(PlayPosition) = 0;
    virtual void notify_call_king(KingCall) = 0;
    virtual std::array<Cards, 2> get_talon() = 0;
    virtual void notify_talon(std::array<Cards, 2> const& talon) = 0;
    virtual bool choose_concede(PlayPosition) = 0;
    virtual void notify_concede() = 0;
    virtual uint8_t choose_talon_half(PlayPosition) = 0;
    virtual void notify_talon_choice(uint8_t) = 0;
    virtual Cards discard(PlayPosition) = 0;
    virtual void notify_discard(Cards) = 0;
    virtual std::vector<Announcement> announce(PlayPosition) = 0;
    virtual void notify_announcements(std::vector<Announcement>) = 0;
    virtual void notify_announcements_done() = 0;
    virtual Card play_card(PlayPosition) = 0;
    virtual void notify_play_card(PlayPosition, Card) = 0;
    virtual Cards get_ouvert(PlayPosition) = 0;
    virtual void notify_ouvert(Cards const&) = 0;
    virtual void notify_invalid(PlayPosition, std::string const&) = 0;
};

}

#endif // KONIG_ORACLE_HPP

