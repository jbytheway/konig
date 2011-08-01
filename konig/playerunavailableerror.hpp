#ifndef KONIG_PLAYERUNAVAILABLEERROR_HPP
#define KONIG_PLAYERUNAVAILABLEERROR_HPP

#include <stdexcept>

namespace konig {

class PlayerUnavailableError : std::runtime_error {
  public:
    PlayerUnavailableError() :
      std::runtime_error("error in attempt to get info from player")
    {}
};

}

#endif // KONIG_PLAYERUNAVAILABLEERROR_HPP

