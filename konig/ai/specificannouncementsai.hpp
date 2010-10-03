#ifndef KONIG_AI__SPECIFICANNOUNCEMENTSAI_HPP
#define KONIG_AI__SPECIFICANNOUNCEMENTSAI_HPP

#include <konig/ai/announcementai.hpp>
#include <konig/ai/fateai.hpp>

namespace konig { namespace ai {

class SpecificAnnouncementsAi : public AnnouncementAi {
  public:
    virtual std::vector<Announcement> announce(FateAi const&);
};

}}

#endif // KONIG_AI__SPECIFICANNOUNCEMENTSAI_HPP

