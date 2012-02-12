#ifndef KONIG_BIDDINGSEQUENCE_HPP
#define KONIG_BIDDINGSEQUENCE_HPP

#include <boost/tuple/tuple.hpp>

#include <konig/contracts.hpp>
#include <konig/player.hpp>
#include <konig/oracle.hpp>

namespace konig {

class KONIG_API BiddingSequence {
  public:
    BiddingSequence(const Contracts& contracts) :
      contracts_(contracts)
    {}

    boost::tuple<Contract const&, PlayPosition>
    get_bids(Players const&);

    boost::tuple<Contract const&, PlayPosition>
    get_bids(Oracle&);
  private:
    Contracts const& contracts_;
};

}

#endif // KONIG_BIDDINGSEQUENCE_HPP

