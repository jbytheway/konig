#ifndef KONIG_AI__MINIMALANNOUNCEMENTAI_HPP
#define KONIG_AI__MINIMALANNOUNCEMENTAI_HPP

#include <announcementai.hpp>

namespace konig { namespace ai {

class MinimalAnnouncementAi : public AnnouncementAi {
  public:
    virtual void reset(FateAi const&);
    virtual KingCall call_king(FateAi const&);
    virtual uint8_t choose_talon_half(FateAi const&);
    virtual Cards discard(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&);
  private:
    bool first_announcements_;
};

}}

#endif // KONIG_AI__MINIMALANNOUNCEMENTAI_HPP

