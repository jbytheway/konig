#ifndef KONIG_PLAY_GAME_HPP
#define KONIG_PLAY_GAME_HPP

#include <vector>

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>

#include <konig/ruleset.hpp>
#include <konig/deal.hpp>
#include <konig/players.hpp>
#include <konig/outcome.hpp>

namespace konig {

KONIG_API PlayResult play_game(
  const Ruleset&,
  const Players&,
  Deal,
  std::ostream* debug_stream = NULL
);

template<typename ForwardRange>
KONIG_API PlayResult play_game(
  const Ruleset& rules,
  const ForwardRange& players_r,
  Deal deal,
  std::ostream* debug_stream = NULL
)
{
  Players players(players_r);
  return play_game(rules, players, std::move(deal), debug_stream);
}

KONIG_API PlayResult play_game(
  const Ruleset&,
  Oracle&
);

}

#endif // KONIG_PLAY_GAME_HPP

