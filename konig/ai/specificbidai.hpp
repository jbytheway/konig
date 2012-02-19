#ifndef KONIG_AI_SPECIFICBIDAI_HPP
#define KONIG_AI_SPECIFICBIDAI_HPP

#include <konig/ai/bidai.hpp>

namespace konig { namespace ai {

class SpecificBidAi : public BidAi {
  public:
    SpecificBidAi(std::string const& description);

    virtual void reset(Ai const&);
    virtual Bid bid(Ai const&);
  private:
    std::string bid_name_;
    Bid bid_;
};

}}

#endif // KONIG_AI_SPECIFICBIDAI_HPP

