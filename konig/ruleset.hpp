#ifndef KONIG__RULESET_HPP
#define KONIG__RULESET_HPP

#include <konig/contracts.hpp>

namespace konig {

class Ruleset {
  public:
    Ruleset() {}
    explicit Ruleset(const Contracts& contracts) :
      contracts_(contracts)
    {}

    const Contracts& contracts() const { return contracts_; }

    static Ruleset solodreier_only();
  private:
    Contracts contracts_;
};

}

#endif // KONIG__RULESET_HPP

