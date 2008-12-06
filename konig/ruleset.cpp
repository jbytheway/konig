#include <konig/ruleset.hpp>

namespace konig {

Ruleset Ruleset::solodreier_only()
{
  return Ruleset(Contracts(Contract::solodreier()));
}

}

