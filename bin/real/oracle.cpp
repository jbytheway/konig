#include "oracle.hpp"

#include "commandhandler.hpp"
#include "uimodes.hpp"

namespace konig { namespace real {

Oracle::Oracle(CommandHandler& handler, ai::Ai::Ptr ai) :
  handler_(handler),
  ai_(std::move(ai)),
  position_(position_max)
{
}

void Oracle::init()
{
  position_ = handler_.get_from_user<uiModes::PlayPosition>();
}

}}

