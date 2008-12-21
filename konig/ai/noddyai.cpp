#include <konig/ai/noddyai.hpp>

namespace konig { namespace ai {

int NoddyAi::bid() {
  if (last_non_pass_ == -1) {
    return rules_.contracts().index_of_contract("solodreier");
  }
  return -1;
}

KingCall NoddyAi::call_king() {
  throw std::logic_error("not implemented");
}

uint8_t NoddyAi::choose_talon_half() {
  throw std::logic_error("not implemented");
}

Cards NoddyAi::discard() {
  throw std::logic_error("not implemented");
}

std::vector<Announcement> NoddyAi::announce() {
  return std::vector<Announcement>();
}

Card NoddyAi::play_card() {
  assert(!hand_.empty());
  Cards legal = legal_plays();
  std::uniform_int<size_t> card_chooser(0, legal.size()-1);
  size_t which = card_chooser(random_engine_);
  Cards::iterator i = legal.begin();
  std::advance(i, which);
  return *i;
}

}}

