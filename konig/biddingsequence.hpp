#ifndef KONIG__BIDDINGSEQUENCE_HPP
#define KONIG__BIDDINGSEQUENCE_HPP

#include <boost/tuple/tuple.hpp>

#include <konig/contracts.hpp>
#include <konig/player.hpp>

namespace konig {

class BiddingSequence {
  public:
    BiddingSequence(const Contracts& contracts) :
      contracts_(contracts)
    {}

    boost::tuple<boost::shared_ptr<Contract const>, PlayPosition>
    get_bids(const std::vector<Player::Ptr>&);
  private:
    const Contracts& contracts_;
    std::vector<Bid> bids_;
};

}

#endif // KONIG__BIDDINGSEQUENCE_HPP

