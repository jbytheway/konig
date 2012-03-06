#ifndef KONIG_AI_LINEARMODELBIDAI_HPP
#define KONIG_AI_LINEARMODELBIDAI_HPP

#include <list>

#include <konig/ai/bidai.hpp>
#include <konig/ai/linearpredictor.hpp>

namespace konig { namespace ai {

class LinearModelBidAi : public BidAi {
  public:
    LinearModelBidAi(std::string const& description);

    virtual void reset(Ai const&);
    virtual Bid bid(Ai const&);
  private:
    std::vector<Feature::Ptr> features_;
    std::list<std::string> lines_;
    std::map<Bid, LinearPredictor> predictors_;
    Bid bid_;
};

}}

#endif // KONIG_AI_LINEARMODELBIDAI_HPP

