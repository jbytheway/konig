#ifndef KONIG__PLAYER_HPP
#define KONIG__PLAYER_HPP

#include <boost/shared_ptr.hpp>

#include <konig/ruleset.hpp>
#include <konig/cards.hpp>
#include <konig/playposition.hpp>

namespace konig {

class Player {
  public:
    typedef boost::shared_ptr<Player> Ptr;

    virtual void start_game(const Ruleset&, PlayPosition, const Cards&) = 0;
    virtual int bid() = 0;
    virtual void notify_bid(int) = 0;
};

}

#endif // KONIG__PLAYER_HPP

