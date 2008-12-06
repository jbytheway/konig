#ifndef KONIG__POSITIVECONTRACT_HPP
#define KONIG__POSITIVECONTRACT_HPP

#include <konig/contract.hpp>

namespace konig {

class PositiveContract : public Contract {
  virtual Outcome play();
};

}

#endif // KONIG__POSITIVECONTRACT_HPP

