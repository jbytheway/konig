#ifndef KONIG_BIDDINGSEQUENCE_HPP
#define KONIG_BIDDINGSEQUENCE_HPP

#include <boost/tuple/tuple.hpp>

#include <konig/contracts.hpp>
#include <konig/player.hpp>

namespace konig {

class KONIG_API BiddingSequence {
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

#endif // KONIG_BIDDINGSEQUENCE_HPP

