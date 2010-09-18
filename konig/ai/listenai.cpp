#include <konig/ai/listenai.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace ai {

Bid ListenAi::bid()
{
  KONIG_FATAL("not implemented");
}

KingCall ListenAi::call_king()
{
  KONIG_FATAL("not implemented");
}

uint8_t ListenAi::choose_talon_half()
{
  KONIG_FATAL("not implemented");
}

Cards ListenAi::discard()
{
  KONIG_FATAL("not implemented");
}

std::vector<Announcement> ListenAi::announce()
{
  KONIG_FATAL("not implemented");
}

Card ListenAi::play_card()
{
  KONIG_FATAL("not implemented");
}

}}

