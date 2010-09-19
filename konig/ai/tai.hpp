#ifndef KONIG_AI__TAI_HPP
#define KONIG_AI__TAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class TAi : public PlayAi {
  public:
    virtual void reset(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&);
    virtual Card play_card(FateAi const&);
};

}}

#endif // KONIG_AI__TAI_HPP

