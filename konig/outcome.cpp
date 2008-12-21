#include <konig/outcome.hpp>

#include <konig/contract.hpp>

namespace konig {

Outcome::Outcome(Contract::Ptr contract) :
  contract_(std::move(contract))
{}

void Outcome::add(bool offence, Feat f, Announcedness an, Achievement ac)
{
  if (an == Announcedness::unannounced && ac == Achievement::neutral)
    return;
  // If it was announced and didn't make, then it's off
  if (ac == Achievement::neutral)
    ac = Achievement::off;
  results_[std::make_pair(f, offence)] = std::make_pair(an, ac);
}

}

