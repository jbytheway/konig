#ifndef KONIG_AI__SDOFFENCEAI_HPP
#define KONIG_AI__SDOFFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class SdOffenceAi : public PlayAi {
  public:
    SdOffenceAi();

    virtual Card play_card(Ai const&);
};

}}

#endif // KONIG_AI__SDOFFENCEAI_HPP

