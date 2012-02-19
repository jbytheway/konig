#include <konig/biddingsequence.hpp>

#include <list>

#include <boost/format.hpp>
#include <boost/bind.hpp>

#include <konig/contract.hpp>
#include <konig/players.hpp>

namespace konig {

namespace {
  struct Bidder {
    virtual Bid bid() const = 0;
    virtual void notify_invalid(std::string) const = 0;
  };

  struct PlayerBidder : Bidder {
    PlayerBidder(Player& player) : player_(&player) {}

    virtual Bid bid() const { return player_->bid(); }
    virtual void notify_invalid(std::string s) const {
      player_->notify_invalid(std::move(s));
    }

    Player* player_;
  };

  struct OracleBidder : Bidder {
    OracleBidder(Oracle& oracle, PlayPosition position) :
      oracle_(&oracle),
      position_(position)
    {}

    virtual Bid bid() const { return oracle_->bid(position_); }
    virtual void notify_invalid(std::string s) const {
      oracle_->notify_invalid(position_, std::move(s));
    }

    Oracle* oracle_;
    PlayPosition position_;
  };

  Bid get_bid(
      Bidder const& bidder,
      bool const forehand,
      Bid const current,
      bool const final_bid,
      Bid const reserved_bids,
      Bid const upper_bound
    ) {
    while (1) {
      Bid bid = bidder.bid();
      // Ensure it's in a sane range
      if (bid >= Bid::pass && bid < upper_bound) {
        if (final_bid) {
          // This is when forehand chooses between the reserved bids
          if (!bid.is_pass() && bid < reserved_bids) {
            return bid;
          }
          bidder.notify_invalid(
            "invalid final bid; must select from reserved bids"
          );
        } else if (!(bid.is_pass() || bid >= current)) {
          bidder.notify_invalid(
            "bid must match or beat previous bid (or be pass)"
          );
        } else if (bid == current && !forehand) {
          bidder.notify_invalid(
            "only forehand may hold a previous bid"
          );
        } else if (bid.is_pass() && current.is_pass()) {
          bidder.notify_invalid(
            "the opening bid may not be pass"
          );
        } else if (bid<reserved_bids && !bid.is_pass() && !current.is_pass()) {
          bidder.notify_invalid(
            "only forehand may use the reserved bids"
          );
        } else if (bid < reserved_bids && bid > Bid(0)) {
          bidder.notify_invalid(
            "to use a reserved bid, open with the lowest"
          );
        } else {
          return bid;
        }
      } else {
        auto message = str(boost::format("bid %d out of range")%bid.value());
        bidder.notify_invalid(message);
      }
    }
  }

  struct Bidders {
    virtual Bidder const& operator[](size_t) const = 0;
    virtual void notify_bid(PlayPosition, Bid) const = 0;
    virtual void notify_contract_established(Bid) const = 0;
  };

  struct PlayersBidders : Bidders {
    PlayersBidders(Players const& players) :
      players_(players)
    {
      for (auto& p : players) {
        bidders_.emplace_back(p);
      }
    }

    virtual Bidder const& operator[](size_t i) const {
      assert(i < 4);
      return bidders_[i];
    }

    virtual void notify_bid(PlayPosition const position, Bid const bid) const {
      std::for_each(
        players_.begin(), players_.end(),
        boost::bind(&Player::notify_bid, _1, position, bid)
      );
    }

    virtual void notify_contract_established(Bid const bid) const {
      std::for_each(
        players_.begin(), players_.end(),
        boost::bind(&Player::notify_contract_established, _1, bid)
      );
    }

    Players const& players_;
    std::vector<PlayerBidder> bidders_;
  };

  struct OracleBidders : Bidders {
    OracleBidders(Oracle& oracle) :
      oracle_(oracle)
    {
      for (PlayPosition p=position_min; p!=position_max; ++p) {
        bidders_.emplace_back(oracle, p);
      }
    }

    virtual Bidder const& operator[](size_t i) const {
      assert(i < 4);
      return bidders_[i];
    }

    virtual void notify_bid(PlayPosition const position, Bid const bid) const {
      oracle_.notify_bid(position, bid);
    }

    virtual void notify_contract_established(Bid const bid) const {
      oracle_.notify_contract_established(bid);
    }

    Oracle& oracle_;
    std::vector<OracleBidder> bidders_;
  };

  boost::tuple<Contract const&, PlayPosition>
  get_bids_impl(Contracts const& contracts, Bidders const& bidders)
  {
    bool player_has_passed[4] = {0};
    Bid const bid_bound(contracts.size());
    Bid const reserved_bids(contracts.reserved_bids());
    // first bid is compulsory
    Bid current =
      get_bid(bidders[0], true, Bid::pass, false, reserved_bids, bid_bound);
    PlayPosition current_declarer = position_forehand;

    bidders.notify_bid(position_forehand, current);

    size_t num_passes = 0;
    PlayPosition player = position_forehand;
    while (num_passes < 3) {
      Bid bid;
      ++player;
      player %= 4;
      if (player_has_passed[player]) {
        bid = Bid::pass;
      } else {
        bid = get_bid(
          bidders[player], player == 0, current, false,
          reserved_bids, bid_bound
        );
      }
      if (bid.is_pass()) {
        ++num_passes;
        player_has_passed[player] = true;
      } else {
        num_passes = 0;
        current = bid;
        current_declarer = player;
      }

      bidders.notify_bid(PlayPosition(player), bid);
    }

    // Finally if we're in the reserved bids we must find out which one
    if (current < reserved_bids) {
      current =
        get_bid(bidders[0], true, current, true, reserved_bids, bid_bound);
      bidders.notify_bid(position_forehand, current);
    }

    bidders.notify_contract_established(current);

    return {contracts[current], current_declarer};
  }
}

boost::tuple<Contract const&, PlayPosition>
BiddingSequence::get_bids(Players const& players)
{
  PlayersBidders bidders(players);
  return get_bids_impl(contracts_, bidders);
}

boost::tuple<Contract const&, PlayPosition>
BiddingSequence::get_bids(Oracle& oracle)
{
  OracleBidders bidders(oracle);
  return get_bids_impl(contracts_, bidders);
}

}

