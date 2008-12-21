#ifndef KONIG__CONTRACT_HPP
#define KONIG__CONTRACT_HPP

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>

#include <konig/outcome.hpp>
#include <konig/cards.hpp>
#include <konig/player.hpp>
#include <konig/trick.hpp>
#include <konig/announcednesses.hpp>

namespace konig {

class ContractAndAnnouncements;

class Contract : public boost::enable_shared_from_this<Contract> {
  public:
    typedef boost::shared_ptr<Contract> Ptr;

    const std::string& name() const { return name_; }

    virtual Outcome play(
        boost::array<Cards, 4> hands, boost::array<Cards, 2> talon,
        const std::vector<Player::Ptr>& players, PlayPosition declarer_position
      ) = 0;

    virtual bool grants_lead() const = 0;

    virtual bool rising_rule() const = 0;

    virtual Announcednesses initial_announcednesses() const = 0;

    virtual bool valid_first_announcements(
        const std::vector<Announcement>&
      ) const = 0;

    virtual Achievement result_for(const Cards& declarers_cards) = 0;

    std::vector<Trick> play_tricks(
        boost::array<Cards, 4> hands,
        Cards& declarers_cards,
        Cards& defenses_cards,
        const std::vector<Player::Ptr>& players,
        const ContractAndAnnouncements& whole_contract,
        PlayPosition declarer_position,
        bool offence[4]
      );

    static Ptr solodreier();
  protected:
    Contract(std::string name) : name_(std::move(name)) {}

    const std::string name_;
};

}

#endif // KONIG__CONTRACT_HPP

