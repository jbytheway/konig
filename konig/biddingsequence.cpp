#include <konig/biddingsequence.hpp>

#include <boost/bind.hpp>

#include <konig/contract.hpp>

namespace konig {

namespace {
  Bid get_bid(
      Player::Ptr const& player,
      bool const forehand,
      bool const allow_pass,
      Bid const current,
      Bid const upper_bound
    ) {
    while (1) {
      Bid bid = Bid::pass;
      bid = player->bid();
      if (current.is_pass() || bid.is_pass() ||
          (bid >= current && (bid > current || forehand))) {
        if (bid >= Bid::pass && bid < upper_bound &&
            (!bid.is_pass() || allow_pass)) {
          return bid;
        }
      }
      player->notify_invalid_play("invalid bid");
    }
  }
}

boost::tuple<boost::shared_ptr<Contract const>, PlayPosition>
BiddingSequence::get_bids(const std::vector<Player::Ptr>& players)
{
  assert(players.size() == 4);
  bids_.clear();
  bool player_has_passed[4] = {0};
  Bid const bid_bound(contracts_.size());
  // first bid is compulsory
  Bid current = get_bid(players[0], true, false, Bid::pass, bid_bound);

  bids_.push_back(current);
  std::for_each(
      players.begin(), players.end(),
      boost::bind(&Player::notify_bid, _1, position_forehand, current)
    );

  size_t num_passes = 0;
  size_t player = 0;
  while (num_passes < 3) {
    Bid bid;
    player++;
    player %= 4;
    if (player_has_passed[player]) {
      bid = Bid::pass;
    } else {
      bid = get_bid(players[player], player == 0, true, current, bid_bound);
    }
    bids_.push_back(bid);
    if (bid.is_pass()) {
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

