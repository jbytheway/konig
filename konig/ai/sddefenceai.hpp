#ifndef KONIG_AI__SDDEFENCEAI_HPP
#define KONIG_AI__SDDEFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class SdDefenceAi : public PlayAi {
  public:
    virtual void reset(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
    Card play_low_short(const Cards& unpreserved, const Cards& preserved) const;

    bool guess_master_defender_;

    struct SuitProfile {
      size_t size;
      size_t points;
      Suit suit;

      friend bool operator<(SuitProfile const& l, SuitProfile const& r) {
        return std::make_tuple(l.size, l.points) <
          std::make_tuple(r.size, r.points);
      }
    };

    typedef std::vector<SuitProfile> SuitProfiles;
    SuitProfiles suit_profiles_;
};

}}

#endif // KONIG_AI__SDDEFENCEAI_HPP

