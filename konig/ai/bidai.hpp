#ifndef KONIG_AI__BIDAI_HPP
#define KONIG_AI__BIDAI_HPP

#include <konig/ai/ai.hpp>

namespace konig { namespace ai {

class BidAi {
  public:
    typedef boost::shared_ptr<BidAi> Ptr;

    static Ptr create(std::string const& description);

    virtual ~BidAi() = 0;

    virtual Bid bid(Ai const&) = 0;
  protected:
    BidAi() = default;
    BidAi(BidAi const&) = default;
    BidAi& operator=(BidAi const&) = default;
};

}}

#endif // KONIG_AI__BIDAI_HPP

