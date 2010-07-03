#include <konig/biddingsequence.hpp>

#include <boost/bind.hpp>

#include <konig/contract.hpp>

namespace konig {

namespace {
  Bid get_bid(
      Player::Ptr const& player,
      bool const forehand,
      Bid const current,
      bool const final_bid,
      Bid const reserved_bids,
      Bid const upper_bound
    ) {
    while (1) {
      Bid bid = player->bid();
      // Ensure it's in a sane range
      if (bid >= Bid::pass && bid < upper_bound) {
        if (final_bid) {
          // This is when forehand chooses between the reserved bids
          if (!bid.is_pass() && bid < reserved_bids) {
            return bid;
          }
          player->notify_invalid_play(
            "invalid final bid; must select from reserved bids"
          );
        } else if (!(bid.is_pass() || bid >= current)) {
          player->notify_invalid_play(
            "bid must match or beat prefvious bid (or be pass)"
          );
        } else if (bid == current && !forehand) {
          player->notify_invalid_play(
            "only forehand may hold a previous bid"
          );
        } else if (bid.is_pass() && current.is_pass()) {
          player->notify_invalid_play(
            "the opening bid may not be pass"
          );
        } else if (bid<reserved_bids && !bid.is_pass() && !current.is_pass()) {
          player->notify_invalid_play(
            "only forehand may use the reserved bids"
          );
        } else if (bid < reserved_bids && bid > Bid(0)) {
          player->notify_invalid_play(
            "to use a reserved bid, open with the lowest"
          );
        } else {
          return bid;
        }
      } else {
        player->notify_invalid_play("bid out of range");
      }
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
  Bid const reserved_bids(contracts_.reserved_bids());
  // first bid is compulsory
  Bid current =
    get_bid(players[0], true, Bid::pass, false, reserved_bids, bid_bound);

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
      bid = get_bid(
          players[player], player == 0, current, false,
          reserved_bids, bid_bound
        );
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

  // Finally if we're in the reserved bids we must find out which one
  if (current < reserved_bids) {
    current =
      get_bid(players[0], true, current, true, reserved_bids, bid_bound);
    bids_.push_back(current);
    std::for_each(
        players.begin(), players.end(),
        boost::bind(&Player::notify_bid, _1, position_forehand, current)
      );
  }

  std::for_each(
      players.begin(), players.end(),
      boost::bind(&Player::notify_contract_established, _1, current)
    );

  return contracts_[current];
}

}

