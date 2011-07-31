#include "oracle.hpp"

namespace konig { namespace real {

Oracle::Oracle(CommandHandler& handler, ai::Ai::Ptr ai) :
  handler_(handler),
  ai_(std::move(ai))
{
}

}}

