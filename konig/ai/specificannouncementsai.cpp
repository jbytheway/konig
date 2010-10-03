#include <konig/ai/specificannouncementsai.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>

#include <konig/ai/aierror.hpp>

namespace konig { namespace ai {

SpecificAnnouncementsAi::SpecificAnnouncementsAi(
  std::string const& announcement_sequence
)
{
  if (!announcement_sequence.empty()) {
    std::vector<std::string> chunks;
    boost::algorithm::split(
      chunks, announcement_sequence, arg1 == ':',
      boost::algorithm::token_compress_off
    );
    BOOST_FOREACH(auto const& chunk, chunks) {
      std::vector<std::string> subchunks;
      boost::algorithm::split(
        subchunks, chunk, arg1 == '/',
        boost::algorithm::token_compress_on
      );
      std::vector<Announcement> this_set;
      BOOST_FOREACH(auto const& subchunk, subchunks) {
        Announcement a;
        if (!Announcement::from_string(a, subchunk)) {
          throw AiError("invalid announcement: '"+subchunk+"'");
        }
        this_set.push_back(a);
      }
      announcements_.push_back(this_set);
    }
  }
  next_announcement_ = announcements_.begin();
}

std::vector<Announcement> SpecificAnnouncementsAi::announce(FateAi const&)
{
  if (next_announcement_ == announcements_.end()) return {};
  std::vector<Announcement> a = *next_announcement_;
  ++next_announcement_;
  return a;
}

}}

