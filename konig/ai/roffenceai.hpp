#ifndef KONIG_AI__ROFFENCEAI_HPP
#define KONIG_AI__ROFFENCEAI_HPP

#include <konig/ai/offenceai.hpp>

namespace konig { namespace ai {

class ROffenceAi : public OffenceAi {
  public:
    ROffenceAi();
    virtual KingCall call_king(FateAi const&);
    virtual uint8_t choose_talon_half(FateAi const&);
    virtual Cards discard(FateAi const&);
  private:
};

}}

#endif // KONIG_AI__ROFFENCEAI_HPP

