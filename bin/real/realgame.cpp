#include "realgame.hpp"

#include <konig/biddingsequence.hpp>

namespace konig { namespace real {

RealGame::RealGame(Ruleset const& rules, Oracle& oracle) :
  rules_(rules),
  oracle_(oracle)
{
}

PlayResult RealGame::play()
{
  oracle_.init();

  Cards hand = oracle_.get_hand();
  oracle_.start_game(rules_, hand);

  BiddingSequence bids(rules_.contracts());
  Contract::ConstPtr contract;
  PlayPosition declarer;
  boost::tie(contract, declarer) = bids.get_bids(oracle_);
  assert(declarer < 4);

  return contract->play(oracle_, declarer);
}

}}

