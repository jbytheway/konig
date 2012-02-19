#ifndef KONIG_AI_FORWARDINGAI_HPP
#define KONIG_AI_FORWARDINGAI_HPP

#include <konig/ai/ai.hpp>
#include <konig/ai/bidai.hpp>
#include <konig/ai/announcementai.hpp>
#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class ForwardingAi : public FateAi {
  public:
    ForwardingAi(std::string const& args);
    ForwardingAi(BidAi::Ptr, AnnouncementAi::Ptr, PlayAi::Ptr);

    virtual void notify_call_king(KingCall);

    virtual Bid bid();
    virtual KingCall call_king();
    virtual bool choose_concede();
    virtual uint8_t choose_talon_half();
    virtual Cards discard();
    virtual std::vector<Announcement> announce();
    virtual Card play_card();

    virtual void game_start_hook();
    virtual void contract_established_hook();
    virtual void play_start_hook();
    virtual void trick_complete_hook();
  private:
    PlayAi::Ptr pick_auto_ai();

    bool debug_;
    bool auto_play_;
    BidAi::Ptr bidder_;
    AnnouncementAi::Ptr announcer_;
    PlayAi::Ptr player_;
};

}}

#endif // KONIG_AI_FORWARDINGAI_HPP

