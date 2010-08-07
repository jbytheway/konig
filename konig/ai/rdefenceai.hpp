#ifndef KONIG_AI__RDEFENCEAI_HPP
#define KONIG_AI__RDEFENCEAI_HPP

#include <konig/ai/defenceai.hpp>

namespace konig { namespace ai {

class RDefenceAi : public DefenceAi {
  public:
    virtual std::vector<Announcement> announce(FateAi const&);
  private:
};

}}

#endif // KONIG_AI__RDEFENCEAI_HPP

