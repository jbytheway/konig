#ifndef KONIG_AI__ROFFENCEAI_HPP
#define KONIG_AI__ROFFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class ROffenceAi : public PlayAi {
  public:
    virtual std::vector<Announcement> announce(FateAi const&);
    virtual void play_start(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
};

}}

#endif // KONIG_AI__ROFFENCEAI_HPP

