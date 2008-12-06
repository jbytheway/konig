#include <konig/game.hpp>

#include <konig/biddingsequence.hpp>

namespace konig {

Outcome Game::play() const
{
  for (PlayPosition i=position_forehand; i<position_max; ++i) {
    players_[i]->start_game(rules_, i, hands_[i]);
  }
  BiddingSequence bids(rules_.contracts());
  Contract::Ptr contract = bids.get_bids(players_);

  return contract->play();
}

}

