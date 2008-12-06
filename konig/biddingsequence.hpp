#ifndef KONIG__BIDDINGSEQUENCE_HPP
#define KONIG__BIDDINGSEQUENCE_HPP

#include <konig/contracts.hpp>
#include <konig/player.hpp>

namespace konig {

class BiddingSequence {
  public:
    BiddingSequence(const Contracts& contracts) :
      contracts_(contracts)
    {}

    Contract::Ptr get_bids(const std::vector<Player::Ptr>&);
  private:
    const Contracts& contracts_;
    std::vector<int> bids_;
};

}

#endif // KONIG__BIDDINGSEQUENCE_HPP

