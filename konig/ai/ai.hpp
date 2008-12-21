#ifndef KONIG_AI__AI_HPP
#define KONIG_AI__AI_HPP

#include <boost/array.hpp>

#include <konig/player.hpp>
#include <konig/trick.hpp>
#include <konig/contractandannouncements.hpp>

namespace konig { namespace ai {

class Ai : public Player {
  public:
    typedef boost::shared_ptr<Ai> Ptr;

    static Ptr create(const std::string& description);

    Ai();

    virtual void start_game(Ruleset, PlayPosition, Cards hand);
    virtual void notify_bid(PlayPosition, int);
    virtual void notify_call_king(KingCall);
    virtual void notify_talon(const boost::array<Cards, 2>& talon);
    virtual void notify_talon_choice(uint8_t);
    virtual void notify_discard(Cards);
    virtual void notify_announcements(std::vector<Announcement>);
    virtual void notify_play_card(PlayPosition, Card);

    Cards legal_plays() const;
  protected:
    Ruleset rules_;
    PlayPosition position_;
    Cards hand_;
    std::vector<int> bidding_;
    int last_non_pass_;
    PlayPosition declarer_;
    bool offence_;
    ContractAndAnnouncements contract_;
    KingCall king_call_;
    boost::array<Cards, 2> talon_;
    Cards accepted_;
    Cards rejected_;
    Cards discard_;
    std::vector<Trick> tricks_;
};

}}

#endif // KONIG_AI__AI_HPP

