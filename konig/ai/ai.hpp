#ifndef KONIG_AI__AI_HPP
#define KONIG_AI__AI_HPP

#include <boost/array.hpp>
#include <boost/optional.hpp>

#include <konig/player.hpp>
#include <konig/trick.hpp>
#include <konig/contractandannouncements.hpp>
#include <konig/ai/api.hpp>

namespace konig { namespace ai {

class KONIG_AI_API Ai : public Player {
  public:
    typedef boost::shared_ptr<Ai> Ptr;

    static Ptr create(const std::string& description);

    Ai();

    virtual void start_game(Ruleset, PlayPosition, Cards hand);
    virtual void notify_bid(PlayPosition, Bid);
    virtual void notify_contract_established(Bid bid);
    virtual void notify_call_king(KingCall);
    virtual void notify_talon(const std::array<Cards, 2>& talon);
    virtual void notify_concede();
    virtual void notify_talon_choice(uint8_t);
    virtual void notify_discard(Cards);
    virtual void notify_announcements(std::vector<Announcement>);
    virtual void notify_play_card(PlayPosition, Card);
    virtual void notify_invalid(std::string);

    virtual void game_start_hook() {}
    virtual void contract_established_hook() {}
    virtual void play_start_hook() {}
    virtual void trick_complete_hook() {}

    Ruleset const& rules() const { return rules_; }
    PlayPosition position() const { return position_; }
    Cards const& hand() const { return hand_; }
    std::vector<Bid> const& bidding() const { return bidding_; }
    Bid last_non_pass() const { return last_non_pass_; }
    PlayPosition declarer() const { return declarer_; }
    bool offence() const { return offence_; }
    ContractAndAnnouncements const& contract() const { return contract_; }
    std::array<Cards, 2> const& talon() const { return talon_; }
    Cards const& accepted() const { return accepted_; }
    Cards const& rejected() const { return rejected_; }
    std::vector<Trick> const& tricks() const { return tricks_; }

    uint8_t trick_number() const;
    uint8_t guess_num_offence() const;
    Cards legal_plays() const;
    boost::optional<Card> relevant_bird() const;
  private:
    Ruleset rules_;
    PlayPosition position_;
    Cards hand_;
    std::vector<Bid> bidding_;
    Bid last_non_pass_;
    PlayPosition declarer_;
    bool offence_;
    ContractAndAnnouncements contract_;
    KingCall king_call_;
    std::array<Cards, 2> talon_;
    bool called_king_in_talon_;
    Cards accepted_;
    Cards rejected_;
    Cards discard_;
    std::vector<Trick> tricks_;
};

}}

#endif // KONIG_AI__AI_HPP

