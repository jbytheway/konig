#ifndef KONIG_AI__STRONGSDDEFENCEAI_HPP
#define KONIG_AI__STRONGSDDEFENCEAI_HPP

#include <random>

#include <konig/ai/ai.hpp>

namespace konig { namespace ai {

class StrongSdDefenceAi : public Ai {
  public:
    virtual void game_start_hook();
    virtual Bid bid();
    virtual KingCall call_king();
    virtual uint8_t choose_talon_half();
    virtual Cards discard();
    virtual std::vector<Announcement> announce();
    virtual Card play_card();

    Card play_low_short(const Cards& unpreserved, const Cards& preserved) const;
  private:
    typedef std::vector<std::tuple<size_t, size_t, Suit> > SizesOfSuits;
    SizesOfSuits sizes_of_suits_;
    Cards cards_to_preserve_;
    Suit longest_suit_;
    std::mt19937 random_engine_;
};

}}

#endif // KONIG_AI__STRONGSDDEFENCEAI_HPP

