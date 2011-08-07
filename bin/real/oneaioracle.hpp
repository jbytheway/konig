#ifndef KONIG_REAL_ONEAIORACLE_HPP
#define KONIG_REAL_ONEAIORACLE_HPP

#include <konig/oracle.hpp>

namespace konig { namespace real {

class OneAiOracle : public Oracle {
  public:
    virtual void init() = 0;
    virtual Cards get_hand() = 0;
    virtual void start_game(Ruleset const&, Cards const& hand) = 0;
    // ...lots more members in Oracle...
    virtual void notify_result(PlayResult const&) = 0;
};

}}

#endif // KONIG_REAL_ONEAIORACLE_HPP

