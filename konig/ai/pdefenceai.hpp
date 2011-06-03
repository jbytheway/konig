#ifndef KONIG_AI__PDEFENCEAI_HPP
#define KONIG_AI__PDEFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class PDefenceAi : public PlayAi {
  public:
    virtual void reset(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
    bool tried_low_trump_;
};

}}

#endif // KONIG_AI__PDEFENCEAI_HPP

