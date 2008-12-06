#include <konig/contract.hpp>

#include <konig/positivecontract.hpp>

namespace konig {

Contract::Ptr Contract::solodreier()
{
  return Ptr(new PositiveContract());
}

}

