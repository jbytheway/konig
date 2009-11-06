#ifndef KONIG_AI__SDOFFENCEAI_HPP
#define KONIG_AI__SDOFFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class SdOffenceAi : public PlayAi {
  public:
    SdOffenceAi();

    virtual std::vector<Announcement> announce(Ai const&);
    virtual void play_start(Ai const&);
    virtual Card play_card(Ai const&);
  private:
    uint8_t num_voids_;
    std::array<int8_t, 4> rippiness_;
    Card lowish_trump_;
};

}}

#endif // KONIG_AI__SDOFFENCEAI_HPP

