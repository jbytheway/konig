#ifndef KONIG_AI__RDEFENCEAI_HPP
#define KONIG_AI__RDEFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class RDefenceAi : public PlayAi {
  public:
    virtual void reset(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
};

}}

#endif // KONIG_AI__RDEFENCEAI_HPP

