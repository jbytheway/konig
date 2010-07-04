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
    typedef int8_t Rippiness;
    typedef uint8_t Count;

    Rippiness const king_rippiness_penalty_;
    std::vector<Count> const trumps_for_unannounced_bird_;
    Count const trumps_out_to_abort_pagat_;
    std::vector<Count> const trumps_out_to_abort_bird_;
    Rippiness const min_desired_rippiness_;
    CardPoints const valuable_card_points_;

    uint8_t num_voids_;
    std::array<Rippiness, 4> rippiness_;
    std::array<bool, 4> void_waiting_;
    Card lowest_trump_to_rough_;
    Card lowest_trump_to_lead_;
};

}}

#endif // KONIG_AI__OFFENCEAI_HPP

