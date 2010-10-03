#ifndef KONIG_AI__SPECIFICANNOUNCEMENTSAI_HPP
#define KONIG_AI__SPECIFICANNOUNCEMENTSAI_HPP

#include <konig/ai/announcementai.hpp>
#include <konig/ai/fateai.hpp>

namespace konig { namespace ai {

class SpecificAnnouncementsAi : public AnnouncementAi {
  public:
    SpecificAnnouncementsAi(std::string const& announcement_sequence = "");
    virtual std::vector<Announcement> announce(FateAi const&);
  private:
    std::vector<std::vector<Announcement>> announcements_;
    std::vector<std::vector<Announcement>>::iterator next_announcement_;
};

}}

#endif // KONIG_AI__SPECIFICANNOUNCEMENTSAI_HPP

