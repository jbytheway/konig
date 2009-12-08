#include <konig/ai/playai.hpp>

#include <konig/ai/nosuchai.hpp>
#include <konig/ai/sdoffenceai.hpp>
#include <konig/ai/sddefenceai.hpp>

namespace konig { namespace ai {

PlayAi::Ptr PlayAi::create(std::string const& description)
{
  std::string::const_iterator equals =
    std::find(description.begin(), description.end(), '=');
  std::string ai_name(description.begin(), equals);
  std::string ai_args(equals, description.end());
  if (!ai_args.empty()) {
    ai_args.erase(ai_args.begin());
  }
  PlayAi::Ptr p;
  if (ai_name == "sdoffence") {
    p.reset(new SdOffenceAi());
  } else if (ai_name == "sddefence") {
    p.reset(new SdDefenceAi());
  } else {
    throw NoSuchAi("PlayAi: "+description);
  }
  return p;
}

PlayAi::~PlayAi() = default;

}}

