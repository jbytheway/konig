#ifndef KONIG_AI_AIERROR_HPP
#define KONIG_AI_AIERROR_HPP

#include <stdexcept>

namespace konig { namespace ai {

class AiError : public std::logic_error {
  public:
    AiError(std::string message) :
      std::logic_error(std::move(message))
    {}
};

}}

#endif // KONIG_AI_AIERROR_HPP

