#ifndef KONIG_AI__SDDEFENCEAI_HPP
#define KONIG_AI__SDDEFENCEAI_HPP

#include <konig/ai/defenceai.hpp>
#include <konig/ai/suitprofile.hpp>

namespace konig { namespace ai {

class SdDefenceAi : public DefenceAi {
  public:
    virtual std::vector<Announcement> announce(FateAi const&);
  private:
};

}}

#endif // KONIG_AI__SDDEFENCEAI_HPP

