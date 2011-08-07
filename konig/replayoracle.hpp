#ifndef KONIG_REPLAYORACLE_HPP
#define KONIG_REPLAYORACLE_HPP

#include <list>

#include <konig/oracle.hpp>

namespace konig {

class ReplayOracle : public Oracle {
  public:
    ReplayOracle(
      Ruleset const&,
      std::vector<std::string> const& bid_names,
      std::string const& king_call_name,
      std::vector<std::string> const& talon_string,
      std::string const& discard_names,
      std::vector<std::string> const& announcement_names,
      std::vector<std::string> const& plays
    );

    virtual Bid bid(PlayPosition);
    virtual void notify_bid(PlayPosition, Bid);
    virtual void notify_contract_established(Bid bid);
    virtual KingCall call_king(PlayPosition);
    virtual void notify_call_king(KingCall);
    virtual std::array<Cards, 2> get_talon();
    virtual void notify_talon(std::array<Cards, 2> const& talon);
    virtual bool choose_concede(PlayPosition);
    virtual void notify_concede();
    virtual uint8_t choose_talon_half(PlayPosition);
    virtual void notify_talon_choice(uint8_t);
    virtual Cards discard(PlayPosition);
    virtual void notify_discard(Cards);
    virtual std::vector<Announcement> announce(PlayPosition);
    virtual void notify_announcements(std::vector<Announcement>);
    virtual void notify_announcements_done();
    virtual Card play_card(PlayPosition);
    virtual void notify_play_card(PlayPosition, Card);
    virtual Cards get_ouvert(PlayPosition);
    virtual void notify_ouvert(Cards const&);
    virtual void notify_invalid(PlayPosition, std::string const&);
  private:
    std::array<std::list<Bid>, 4> bids_;
    boost::optional<KingCall> king_call_;
    std::array<Cards, 2> talon_;
    bool concession_;
    int talon_choice_;
    Cards discard_;
    std::array<std::list<std::vector<Announcement>>, 4> announcements_;
    std::array<std::list<Card>, 4> plays_;
};

}

#endif // KONIG_REPLAYORACLE_HPP

