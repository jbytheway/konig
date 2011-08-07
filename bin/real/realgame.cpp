#include "realgame.hpp"

#include <konig/play_game.hpp>

namespace konig { namespace real {

RealGame::RealGame(Ruleset const& rules, OneAiOracle& oracle) :
  rules_(rules),
  oracle_(oracle)
{
}

PlayResult RealGame::play()
{
  oracle_.init();

  Cards hand = oracle_.get_hand();
  oracle_.start_game(rules_, hand);

  auto result = play_game(rules_, oracle_);
  oracle_.notify_result(result);
  return result;
}

}}

