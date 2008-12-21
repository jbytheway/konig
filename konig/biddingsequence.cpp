#include <konig/biddingsequence.hpp>

#include <boost/bind.hpp>

#include <konig/contract.hpp>

namespace konig {

namespace {
  int get_bid(
      Player::Ptr player,
      bool forehand,
      bool allow_pass,
      int current,
      int num_contracts
    ) {
    while (1) {
      int bid = -1;
      bid = player->bid();
      if (current == -1 || bid == -1 ||
          (bid >= current && (bid > current || forehand))) {
        if (bid >= -1 && bid < num_contracts && (bid != -1 || allow_pass)) {
          return bid;
        }
      }
    }
  }
}

boost::tuple<boost::shared_ptr<Contract>, PlayPosition>
BiddingSequence::get_bids(const std::vector<Player::Ptr>& players)
{
  assert(players.size() == 4);
  bids_.clear();
  bool player_has_passed[4] = {0};
  int current = 0;
  // first bid is compulsory
  current = get_bid(players[0], true, false, -1, contracts_.size());

  bids_.push_back(current);
  std::for_each(
      players.begin(), players.end(),
      boost::bind(&Player::notify_bid, _1, position_forehand, current)
    );

  size_t num_passes = 0;
  size_t player = 0;
  while (num_passes < 3) {
    int bid;
    player++;
    player %= 4;
    if (player_has_passed[player]) {
      bid = -1;
    } else {
      bid =
        get_bid(players[player], player == 0, true, current, contracts_.size());
    }
    bids_.push_back(bid);
    if (bid == -1) {
      ++num_passes;
      player_has_passed[player] = true;
    } else {
      num_passes = 0;
      current = bid;
    }

    std::for_each(
        players.begin(), players.end(),
        boost::bind(&Player::notify_bid, _1, PlayPosition(player), bid)
      );
  }

  return contracts_[current];
}

}

