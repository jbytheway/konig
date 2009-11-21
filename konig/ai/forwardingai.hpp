#ifndef KONIG_AI__FORWARDINGAI_HPP
#define KONIG_AI__FORWARDINGAI_HPP

#include <konig/ai/ai.hpp>
#include <konig/ai/bidai.hpp>
#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class ForwardingAi : public FateAi {
  public:
    ForwardingAi(std::string const& args);

    virtual Bid bid();
    virtual KingCall call_king();
    virtual uint8_t choose_talon_half();
    virtual Cards discard();
    virtual std::vector<Announcement> announce();
    virtual Card play_card();

    virtual void play_start_hook();
    virtual void trick_complete_hook();
  private:
    bool debug_;
    BidAi::Ptr bidder_;
    PlayAi::Ptr player_;
};

}}

#endif // KONIG_AI__FORWARDINGAI_HPP

