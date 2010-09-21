#ifndef KONIG__GAME_HPP
#define KONIG__GAME_HPP

#include <vector>

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>

#include <konig/ruleset.hpp>
#include <konig/deal.hpp>
#include <konig/player.hpp>
#include <konig/outcome.hpp>

namespace konig {

class KONIG_API Game {
  public:
    template<typename ForwardRange>
    Game(const Ruleset& rules, const ForwardRange& players, Deal deal) :
      rules_(rules),
      players_(boost::begin(players), boost::end(players))
    {
      if (players_.size() != 4) {
        throw std::logic_error("must have 4 players");
      }

      for (unsigned int i=0; i<4; ++i) {
        assert(players_[i]);
      }

      deal.copy_hands(hands_.begin());
      deal.copy_talon(talon_.begin());
    }

    PlayResult play(std::ostream* debug_stream = NULL) const;
  private:
    const Ruleset rules_;
    const std::vector<Player::Ptr> players_;
    std::array<Cards, 4> hands_;
    std::array<Cards, 2> talon_;
};

}

#endif // KONIG__GAME_HPP

