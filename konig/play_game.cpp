#include <konig/play_game.hpp>

#include <boost/tuple/tuple.hpp>

#include <konig/biddingsequence.hpp>
#include <konig/contract.hpp>

namespace konig {

PlayResult play_game(
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
  auto const r = bids.get_bids(players);
  Contract const& contract = boost::tuples::get<0>(r);
  PlayPosition declarer = boost::tuples::get<1>(r);
  assert(declarer < 4);

  if (debug_stream) {
    *debug_stream << "contract is " << contract.name() <<
      " by " << declarer << std::endl;
  }

  return contract.play(hands, talon, players, declarer, debug_stream);
}

PlayResult play_game(
  const Ruleset& rules,
  Oracle& oracle
)
{
  BiddingSequence bids(rules.contracts());
  auto const r = bids.get_bids(oracle);
  Contract const& contract = boost::tuples::get<0>(r);
  PlayPosition declarer = boost::tuples::get<1>(r);
  assert(declarer < 4);

  return contract.play(oracle, declarer);
}

}

