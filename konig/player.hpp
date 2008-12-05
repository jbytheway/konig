#ifndef KONIG__PLAYER_HPP
#define KONIG__PLAYER_HPP

#include <boost/shared_ptr.hpp>

#include <konig/core.hpp>

namespace konig {

class Player {
  public:
    typedef boost::shared_ptr<Player> Ptr;
};

}

#endif // KONIG__PLAYER_HPP

