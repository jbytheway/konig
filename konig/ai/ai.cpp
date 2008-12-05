#include <konig/ai/ai.hpp>

#include <konig/ai/noddyai.hpp>

namespace konig { namespace ai {

Ai::Ptr Ai::create(const std::string& /*description*/)
{
  Ai::Ptr p(new NoddyAi());
  return p;
}

}}

