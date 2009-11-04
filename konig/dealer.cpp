#include <konig/dealer.hpp>

#include <konig/completedealer.hpp>
#include <konig/partialdealer.hpp>

namespace konig {

Dealer::Ptr Dealer::create()
{
  Dealer::Ptr p(new CompleteDealer());
  return p;
}

Dealer::Ptr Dealer::create(unsigned long seed)
{
  Dealer::Ptr p(new CompleteDealer(seed));
  return p;
}

Dealer::Ptr Dealer::create(const std::string (& descriptions)[6])
{
  Dealer::Ptr p(new PartialDealer(descriptions));
  return p;
}

Dealer::Ptr Dealer::create(
    const std::string (& descriptions)[6],
    unsigned long seed
  )
{
  Dealer::Ptr p(new PartialDealer(descriptions, seed));
  return p;
}

Dealer::Dealer()
{
  std::random_device device("/dev/urandom");
  random_engine_.seed(device);
}

Dealer::Dealer(unsigned long seed)
{
  random_engine_.seed(seed);
}

}

