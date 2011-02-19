#ifndef KONIG_AI__BDEFENCEAI_HPP
#define KONIG_AI__BDEFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class BDefenceAi : public PlayAi {
  public:
    virtual void reset(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
    bool tried_low_trump_;
};

}}

#endif // KONIG_AI__BDEFENCEAI_HPP

