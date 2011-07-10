#ifndef KONIG_AI__SPECIFICANNOUNCEMENTSAI_HPP
#define KONIG_AI__SPECIFICANNOUNCEMENTSAI_HPP

#include <konig/ai/announcementai.hpp>
#include <konig/ai/fateai.hpp>

namespace konig { namespace ai {

class SpecificAnnouncementsAi : public AnnouncementAi {
  public:
    typedef std::vector<std::vector<Announcement>> Script;

    SpecificAnnouncementsAi(
      std::string const& announcement_sequence = "",
      boost::optional<KingCall> = boost::optional<KingCall>()
    );

    SpecificAnnouncementsAi(
      Script,
      boost::optional<KingCall> = boost::optional<KingCall>()
    );

    virtual KingCall call_king(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&);
  private:
    boost::optional<KingCall> king_call_;
    Script announcements_;
    Script::iterator next_announcement_;
};

}}

#endif // KONIG_AI__SPECIFICANNOUNCEMENTSAI_HPP

