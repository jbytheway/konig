#ifndef KONIG_AI_INVALIDERROR_HPP
#define KONIG_AI_INVALIDERROR_HPP

namespace konig { namespace ai {

class KONIG_AI_API InvalidError : public std::logic_error {
  public:
    InvalidError(std::string const& m) :
      std::logic_error(m)
    {}
};

}}

#endif // KONIG_AI_INVALIDERROR_HPP

