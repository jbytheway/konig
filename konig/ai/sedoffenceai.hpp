#ifndef KONIG_AI_SEDOFFENCEAI_HPP
#define KONIG_AI_SEDOFFENCEAI_HPP

#include <konig/ai/offenceai.hpp>

namespace konig { namespace ai {

class SedOffenceAi : public OffenceAi {
  // Sed in place of 6d (identifiers can't start with digits)
  public:
    SedOffenceAi();
};

}}

#endif // KONIG_AI_SEDOFFENCEAI_HPP

