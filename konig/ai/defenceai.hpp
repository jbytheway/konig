#ifndef KONIG_AI__DEFENCEAI_HPP
#define KONIG_AI__DEFENCEAI_HPP

#include <konig/ai/playai.hpp>
#include <konig/ai/suitprofile.hpp>

namespace konig { namespace ai {

class DefenceAi : public PlayAi {
  public:
    virtual void reset(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
    bool guess_master_defender_;

    typedef std::vector<SuitProfile> SuitProfiles;
    SuitProfiles suit_profiles_;
};

}}

#endif // KONIG_AI__DEFENCEAI_HPP

