#include <konig/partialcards.hpp>

namespace konig {

PartialCards::PartialCards(const std::string& description, size_t size)
{
  fixed_ = Cards::from_string(description);
  if (size < fixed_.size()) {
    throw std::logic_error("too many cards specified");
  }
  num_random_ = size - fixed_.size();
}

}

