#ifndef KONIG_AI__SDDEFENCEAI_HPP
#define KONIG_AI__SDDEFENCEAI_HPP

#include <random>

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class SdDefenceAi : public PlayAi {
  public:
    virtual std::vector<Announcement> announce(FateAi const&);
    virtual void play_start(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
    Card play_low_short(const Cards& unpreserved, const Cards& preserved) const;

    typedef std::vector<std::tuple<size_t, size_t, Suit> > SizesOfSuits;
    SizesOfSuits sizes_of_suits_;
    Cards cards_to_preserve_;
    Suit longest_suit_;
    std::mt19937 random_engine_;
};

}}

#endif // KONIG_AI__SDDEFENCEAI_HPP

