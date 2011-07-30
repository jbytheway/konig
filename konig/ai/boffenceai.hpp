#ifndef KONIG_AI_BOFFENCEAI_HPP
#define KONIG_AI_BOFFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class BOffenceAi : public PlayAi {
  public:
    virtual void reset(FateAi const&);
    virtual Card play_card(FateAi const&);
};

}}

#endif // KONIG_AI_BOFFENCEAI_HPP

