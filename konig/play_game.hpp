#ifndef KONIG_PLAY_GAME_HPP
#define KONIG_PLAY_GAME_HPP

#include <vector>

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>

#include <konig/ruleset.hpp>
#include <konig/deal.hpp>
#include <konig/player.hpp>
#include <konig/outcome.hpp>

namespace konig {

KONIG_API PlayResult play_game(
  const Ruleset&,
  const std::vector<Player::Ptr>&,
  Deal,
  std::ostream* debug_stream = NULL
);

template<typename ForwardRange>
KONIG_API PlayResult play_game(
  const Ruleset& rules,
  const ForwardRange& players,
  Deal deal,
  std::ostream* debug_stream = NULL
)
{
  std::vector<Player::Ptr> players_v(players.begin(), players.end());
  return play_game(rules, players_v, std::move(deal), debug_stream);
}

}

#endif // KONIG_PLAY_GAME_HPP

