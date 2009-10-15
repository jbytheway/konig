#ifndef KONIG_AI__NOSUCHAI_HPP
#define KONIG_AI__NOSUCHAI_HPP

#include <konig/ai/aierror.hpp>

namespace konig { namespace ai {

class NoSuchAi : public AiError {
  public:
    NoSuchAi(std::string const& desc) :
      AiError("no such ai: "+desc)
    {}
};

}}

#endif // KONIG_AI__NOSUCHAI_HPP

