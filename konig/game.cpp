#include <konig/game.hpp>

#include <boost/tuple/tuple.hpp>

#include <konig/biddingsequence.hpp>
#include <konig/contract.hpp>

namespace konig {

boost::tuple<Outcome, std::vector<Trick> >
Game::play(std::ostream* debug_stream) const
{
  for (PlayPosition i=position_forehand; i<position_max; ++i) {
    players_[i]->start_game(rules_, i, hands_[i]);
  }
  BiddingSequence bids(rules_.contracts());
  Contract::ConstPtr contract;
  PlayPosition declarer;
  boost::tie(contract, declarer) = bids.get_bids(players_);
  assert(declarer < 4);

  if (debug_stream) {
    *debug_stream << "contract is " << contract->name() <<
      " by " << declarer << std::endl;
  }

  return contract->play(hands_, talon_, players_, declarer, debug_stream);
}

}

