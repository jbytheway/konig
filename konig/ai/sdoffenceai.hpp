#ifndef KONIG_AI__SDOFFENCEAI_HPP
#define KONIG_AI__SDOFFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class SdOffenceAi : public PlayAi {
  public:
    SdOffenceAi();

    virtual std::vector<Announcement> announce(FateAi const&);
    virtual void play_start(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
    uint8_t num_voids_;
    std::array<int8_t, 4> rippiness_;
    std::array<bool, 4> void_waiting_;
    Card lowest_trump_to_rough_;
    Card lowest_trump_to_lead_;
};

}}

#endif // KONIG_AI__SDOFFENCEAI_HPP

