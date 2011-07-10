#ifndef KONIG_AI__ANNOUNCEMENTAI_HPP
#define KONIG_AI__ANNOUNCEMENTAI_HPP

#include <boost/shared_ptr.hpp>

#include <konig/ai/fateai.hpp>

namespace konig { namespace ai {

class AnnouncementAi {
  public:
    typedef boost::shared_ptr<AnnouncementAi> Ptr;

    static Ptr create(std::string const& description);

    virtual ~AnnouncementAi() = 0;

    virtual void reset(FateAi const&) {};
    virtual KingCall call_king(FateAi const&);
    virtual bool choose_concede(FateAi const&);
    virtual uint8_t choose_talon_half(FateAi const&);
    virtual Cards discard(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&);
  protected:
    AnnouncementAi() {}
    AnnouncementAi(AnnouncementAi const&);
    AnnouncementAi& operator=(AnnouncementAi const&);
};

inline AnnouncementAi::AnnouncementAi(AnnouncementAi const&) = default;
inline AnnouncementAi&
AnnouncementAi::operator=(AnnouncementAi const&) = default;

}}

#endif // KONIG_AI__ANNOUNCEMENTAI_HPP

