#ifndef KONIG_AI__OFFENCEAI_HPP
#define KONIG_AI__OFFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class OffenceAi : public PlayAi {
  public:
    OffenceAi();

    virtual void reset(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
    uint8_t num_voids_;
    std::array<int8_t, 4> rippiness_;
    std::array<bool, 4> void_waiting_;
    Card lowest_trump_to_rough_;
    Card lowest_trump_to_lead_;
};

}}

#endif // KONIG_AI__OFFENCEAI_HPP

