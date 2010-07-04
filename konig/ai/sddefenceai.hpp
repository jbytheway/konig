#ifndef KONIG_AI__SDDEFENCEAI_HPP
#define KONIG_AI__SDDEFENCEAI_HPP

#include <konig/ai/playai.hpp>
#include <konig/ai/suitprofile.hpp>

namespace konig { namespace ai {

class SdDefenceAi : public PlayAi {
  public:
    virtual void reset(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
    Card play_low_short(const Cards& unpreserved, const Cards& preserved) const;

    bool guess_master_defender_;

    typedef std::vector<SuitProfile> SuitProfiles;
    SuitProfiles suit_profiles_;
};

}}

#endif // KONIG_AI__SDDEFENCEAI_HPP

