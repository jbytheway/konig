#ifndef KONIG_AI__FORWARDINGAI_HPP
#define KONIG_AI__FORWARDINGAI_HPP

#include <konig/ai/ai.hpp>
#include <konig/ai/bidai.hpp>
#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class ForwardingAi : public Ai {
  public:
    ForwardingAi(std::string const& args);

    virtual Bid bid();
    virtual KingCall call_king();
    virtual uint8_t choose_talon_half();
    virtual Cards discard();
    virtual std::vector<Announcement> announce();
    virtual Card play_card();
  private:
    BidAi::Ptr bidder_;
    PlayAi::Ptr player_;
};

}}

#endif // KONIG_AI__FORWARDINGAI_HPP

