#ifndef KONIG_AI__LISTENAI_HPP
#define KONIG_AI__LISTENAI_HPP

#include <konig/ai/fateai.hpp>

namespace konig { namespace ai {

class KONIG_AI_API ListenAi : public FateAi {
  public:
    virtual Bid bid();
    virtual KingCall call_king();
    virtual uint8_t choose_talon_half();
    virtual Cards discard();
    virtual std::vector<Announcement> announce();
    virtual Card play_card();
};

}}

#endif // KONIG_AI__LISTENAI_HPP

