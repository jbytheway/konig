#include <konig/ai/announcementai.hpp>

#include <konig/ai/nosuchai.hpp>
#include <konig/ai/specificannouncementsai.hpp>
#include <konig/ai/minimalannouncementai.hpp>

namespace konig { namespace ai {

AnnouncementAi::Ptr AnnouncementAi::create(std::string const& description)
{
  std::string::const_iterator equals =
    std::find(description.begin(), description.end(), '=');
  std::string ai_name(description.begin(), equals);
  std::string ai_args(equals, description.end());
  if (!ai_args.empty()) {
    ai_args.erase(ai_args.begin());
  }
  AnnouncementAi::Ptr p;
  if (ai_name == "specific") {
    p.reset(new SpecificAnnouncementsAi(ai_args));
  } else if (ai_name == "") {
    p.reset(new MinimalAnnouncementAi());
  } else {
    throw NoSuchAi("AnnouncementAi: "+description);
  }
  return p;
}

AnnouncementAi::~AnnouncementAi() = default;

KingCall AnnouncementAi::call_king(FateAi const&)
{
  KONIG_FATAL("not implemented in " << typeid(*this).name());
}

uint8_t AnnouncementAi::choose_talon_half(FateAi const&)
{
  KONIG_FATAL("not implemented in " << typeid(*this).name());
}

Cards AnnouncementAi::discard(FateAi const&)
{
  KONIG_FATAL("not implemented in " << typeid(*this).name());
}

std::vector<Announcement> AnnouncementAi::announce(FateAi const&)
{
  return std::vector<Announcement>();
}

}}

