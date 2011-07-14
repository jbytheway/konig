#ifndef KONIG_DTERM__GAMETRACKER_HPP
#define KONIG_DTERM__GAMETRACKER_HPP

#include <konig/ai/ai.hpp>

namespace konig { namespace dterm {

class CommandHandler;

class GameTracker : public ai::Ai {
  public:
    GameTracker(CommandHandler&);

    virtual void notify_talon(const std::array<Cards, 2>& talon);
    virtual void notify_invalid(std::string);

    virtual void game_start_hook();
    virtual void trick_complete_hook();

    virtual Bid bid();
    virtual KingCall call_king();
    virtual bool choose_concede();
    virtual uint8_t choose_talon_half();
    virtual Cards discard();
    virtual std::vector<Announcement> announce();
    virtual void notify_announcements_done();
    virtual Card play_card();
  private:
    CommandHandler& handler_;
};

}}

#endif // KONIG_DTERM__GAMETRACKER_HPP

