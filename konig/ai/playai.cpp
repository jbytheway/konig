#include <konig/ai/playai.hpp>

#include <konig/ai/nosuchai.hpp>

namespace konig { namespace ai {

PlayAi::Ptr PlayAi::create(std::string const& description)
{
  throw NoSuchAi("PlayAi: "+description);
}

PlayAi::~PlayAi() = default;

}}

