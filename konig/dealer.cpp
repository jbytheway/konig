#include <konig/dealer.hpp>

#include <konig/partialdealer.hpp>

namespace konig {

Dealer::Ptr Dealer::create(const std::string (& descriptions)[6])
{
  Dealer::Ptr p(new PartialDealer(descriptions));
  return p;
}

Dealer::Dealer()
{
  // TODO: initialize random_engine_ properly
}

}

