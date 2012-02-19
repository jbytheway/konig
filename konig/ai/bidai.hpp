#ifndef KONIG_AI_BIDAI_HPP
#define KONIG_AI_BIDAI_HPP

#include <konig/ai/ai.hpp>

namespace konig { namespace ai {

class BidAi {
  public:
    typedef boost::shared_ptr<BidAi> Ptr;

    static Ptr create(std::string const& description);

    virtual ~BidAi() = 0;

    virtual void reset(Ai const&) {}
    virtual Bid bid(Ai const&) = 0;
  protected:
    BidAi() {}
    BidAi(BidAi const&);
    BidAi& operator=(BidAi const&);
};

inline BidAi::BidAi(BidAi const&) = default;
inline BidAi& BidAi::operator=(BidAi const&) = default;

}}

#endif // KONIG_AI_BIDAI_HPP

