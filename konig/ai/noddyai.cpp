#include <konig/ai/noddyai.hpp>

namespace konig { namespace ai {

Bid NoddyAi::bid() {
  if (last_non_pass().is_pass()) {
    return rules().contracts().index_by_name("solodreier");
  }
  return Bid::pass;
}

KingCall NoddyAi::call_king() {
  throw std::logic_error("not implemented");
}

bool NoddyAi::choose_concede() {
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
  Cards legal = legal_plays();
  assert(!legal.empty());
  std::uniform_int_distribution<size_t> card_chooser(0, legal.size()-1);
  size_t which = card_chooser(random_engine_);
  Cards::iterator i = legal.begin();
  std::advance(i, which);
  return *i;
}

}}

