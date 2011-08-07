#include <konig/play_game.hpp>

#include <boost/tuple/tuple.hpp>

#include <konig/biddingsequence.hpp>
#include <konig/contract.hpp>

namespace konig {

KONIG_API PlayResult play_game(
  const Ruleset& rules,
  const std::vector<Player::Ptr>& players,
  Deal deal,
  std::ostream* debug_stream
)
{
  if (players.size() != 4) {
    throw std::logic_error("must have 4 players");
  }

  for (unsigned int i=0; i<4; ++i) {
    assert(players[i]);
  }

  std::array<Cards, 4> hands;
  std::array<Cards, 2> talon;
  deal.copy_hands(hands.begin());
  deal.copy_talon(talon.begin());

  for (PlayPosition i=position_forehand; i<position_max; ++i) {
    players[i]->start_game(rules, i, hands[i]);
  }
  BiddingSequence bids(rules.contracts());
  Contract::ConstPtr contract;
  PlayPosition declarer;
  boost::tie(contract, declarer) = bids.get_bids(players);
  assert(declarer < 4);

  if (debug_stream) {
    *debug_stream << "contract is " << contract->name() <<
      " by " << declarer << std::endl;
  }

  return contract->play(hands, talon, players, declarer, debug_stream);
}

}

