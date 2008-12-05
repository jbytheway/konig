#ifndef KONIG__GAME_HPP
#define KONIG__GAME_HPP

#include <boost/array.hpp>

#include <konig/deal.hpp>
#include <konig/player.hpp>

namespace konig {

class Game {
  public:
    template<typename ForwardRange>
    Game(const ForwardRange& players, Deal deal) :
      players_(boost::begin(players), boost::end(players))
    {
      if (players_.size() != 4) {
        throw std::logic_error("must have 4 players");
      }

      deal.copy_hands(hands_.begin());
      deal.copy_talon(talon_.begin());
    }
  private:
    std::vector<Player::Ptr> players_;
    boost::array<Cards, 4> hands_;
    boost::array<Cards, 2> talon_;
};

}

#endif // KONIG__GAME_HPP

