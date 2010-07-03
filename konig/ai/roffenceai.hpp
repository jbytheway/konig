#ifndef KONIG_AI__ROFFENCEAI_HPP
#define KONIG_AI__ROFFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class ROffenceAi : public PlayAi {
  public:
    virtual void reset(FateAi const&);
    virtual KingCall call_king(FateAi const&);
    virtual uint8_t choose_talon_half(FateAi const&);
    virtual Cards discard(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
};

}}

#endif // KONIG_AI__ROFFENCEAI_HPP

