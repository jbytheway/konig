#ifndef KONIG_DTERM__GAMETRACKER_HPP
#define KONIG_DTERM__GAMETRACKER_HPP

#include <konig/ai/ai.hpp>

namespace konig { namespace dterm {

class CommandHandler;

class GameTracker : public ai::Ai {
  public:
    GameTracker(CommandHandler&);
    virtual Bid bid();
    virtual KingCall call_king();
    virtual uint8_t choose_talon_half();
    virtual Cards discard();
    virtual std::vector<Announcement> announce();
    virtual Card play_card();
  private:
    CommandHandler& handler_;
};

}}

#endif // KONIG_DTERM__GAMETRACKER_HPP

