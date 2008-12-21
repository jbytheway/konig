#ifndef KONIG_AI__NODDYAI_HPP
#define KONIG_AI__NODDYAI_HPP

#include <random>

#include <konig/ai/ai.hpp>

namespace konig { namespace ai {

class NoddyAi : public Ai {
  public:
    virtual int bid();
    virtual KingCall call_king();
    virtual uint8_t choose_talon_half();
    virtual Cards discard();
    virtual std::vector<Announcement> announce();
    virtual Card play_card();
  private:
    std::mt19937 random_engine_;
};

}}

#endif // KONIG_AI__NODDYAI_HPP

