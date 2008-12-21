#include <konig/ruleset.hpp>

#include <konig/contract.hpp>

namespace konig {

Ruleset Ruleset::solodreier_only()
{
  return Ruleset(Contracts(Contract::solodreier()));
}

}

