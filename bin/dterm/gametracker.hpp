#ifndef KONIG_DTERM__GAMETRACKER_HPP
#define KONIG_DTERM__GAMETRACKER_HPP

#include <konig/ai/ai.hpp>

namespace konig { namespace dterm {

class GameTracker : public ai::Ai {
  public:
    virtual int bid();
    virtual KingCall call_king();
    virtual uint8_t choose_talon_half();
    virtual Cards discard();
    virtual std::vector<Announcement> announce();
    virtual Card play_card();
};

}}

#endif // KONIG_DTERM__GAMETRACKER_HPP

