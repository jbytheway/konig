#ifndef KONIG_REAL_TERMINALORACLE_HPP
#define KONIG_REAL_TERMINALORACLE_HPP

#include <konig/ai/ai.hpp>

#include "oneaioracle.hpp"

namespace konig { namespace real {

class CommandHandler;

class TerminalOracle : public OneAiOracle {
  public:
    TerminalOracle(Ruleset const&, CommandHandler&, ai::Ai::Ptr);

    virtual void init();
    virtual Cards get_hand();
    virtual void start_game(Ruleset const&, Cards const& hand);

    virtual Bid bid(PlayPosition);
    virtual void notify_bid(PlayPosition, Bid);
    virtual void notify_contract_established(Bid bid);
    virtual KingCall call_king(PlayPosition);
    virtual void notify_call_king(KingCall);
    virtual std::array<Cards, 2> get_talon();
    virtual void notify_talon(std::array<Cards, 2> const& talon);
    virtual bool choose_concede(PlayPosition);
    virtual void notify_concede();
    virtual uint8_t choose_talon_half(PlayPosition);
    virtual void notify_talon_choice(uint8_t);
    virtual Cards discard(PlayPosition);
    virtual void notify_discard(Cards);
    virtual std::vector<Announcement> announce(PlayPosition);
    virtual void notify_announcements(std::vector<Announcement>);
    virtual void notify_announcements_done();
    virtual Card play_card(PlayPosition);
    virtual void notify_play_card(PlayPosition, Card);
    virtual Cards get_ouvert(PlayPosition);
    virtual void notify_ouvert(Cards const&);
    virtual void notify_result(PlayResult const&);
    virtual void notify_invalid(PlayPosition, std::string const&);
  private:
    Ruleset const& rules_;
    CommandHandler& handler_;
    ai::Ai::Ptr ai_;

    PlayPosition position_;
};

}}

#endif // KONIG_REAL_TERMINALORACLE_HPP

