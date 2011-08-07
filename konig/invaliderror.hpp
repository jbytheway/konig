#ifndef KONIG_INVALIDERROR_HPP
#define KONIG_INVALIDERROR_HPP

#include <konig/api.hpp>

namespace konig {

class KONIG_API InvalidError : public std::logic_error {
  public:
    InvalidError(std::string const& m) :
      std::logic_error(m)
    {}
};

}

#endif // KONIG_INVALIDERROR_HPP

