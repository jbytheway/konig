#ifndef KONIG_REAL_ORACLE_HPP
#define KONIG_REAL_ORACLE_HPP

#include <konig/ai/ai.hpp>

namespace konig { namespace real {

class CommandHandler;

class Oracle {
  public:
    Oracle(CommandHandler&, ai::Ai::Ptr);
  private:
    CommandHandler& handler_;
    ai::Ai::Ptr ai_;
};

}}

#endif // KONIG_REAL_ORACLE_HPP

