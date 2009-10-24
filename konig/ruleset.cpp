#include <konig/ruleset.hpp>

#include <boost/assign/list_of.hpp>

#include <konig/contract.hpp>

namespace konig {

Ruleset Ruleset::cheltenham()
{
  return Ruleset(Contracts(
        boost::assign::list_of
          (Contract::rufer())
          (Contract::trischaken())
          (Contract::sechserdreier())
          (Contract::solo())
          (Contract::piccolo())
          (Contract::besserrufer())
          (Contract::bettel())
          (Contract::dreier())
          (Contract::piccolo_ouvert())
          (Contract::besserdreier())
          (Contract::bettel_ouvert())
          (Contract::solodreier()),
        3
      ));
}

Ruleset Ruleset::solodreier_only()
{
  return Ruleset(Contracts(Contract::solodreier()));
}

}

