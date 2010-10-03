#ifndef KONIG_AI__SPECIFICBIDSAI_HPP
#define KONIG_AI__SPECIFICBIDSAI_HPP

#include <konig/ai/bidai.hpp>

namespace konig { namespace ai {

class SpecificBidsAi : public BidAi {
  public:
    SpecificBidsAi(std::vector<Bid>);
    virtual Bid bid(Ai const&);
  private:
    std::vector<Bid> bids_;
    std::vector<Bid>::iterator next_bid_;
};

}}

#endif // KONIG_AI__SPECIFICBIDSAI_HPP

