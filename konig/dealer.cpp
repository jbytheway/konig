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
  std::random_device device("/dev/urandom");
  random_engine_.seed(device);
}

}

