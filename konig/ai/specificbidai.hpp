#ifndef KONIG_AI__SPECIFICBIDAI_HPP
#define KONIG_AI__SPECIFICBIDAI_HPP

#include <konig/ai/bidai.hpp>

namespace konig { namespace ai {

class SpecificBidAi : public BidAi {
  public:
    SpecificBidAi(std::string const& description);
    virtual Bid bid(Ai const&);
  private:
    std::string bid_name_;
};

}}

#endif // KONIG_AI__SPECIFICBIDAI_HPP
