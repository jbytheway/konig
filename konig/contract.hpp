#ifndef KONIG__CONTRACT_HPP
#define KONIG__CONTRACT_HPP

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/serialization/string.hpp>

#include <konig/outcome.hpp>
#include <konig/cards.hpp>
#include <konig/trick.hpp>
#include <konig/announcement.hpp>
#include <konig/announcednesses.hpp>

namespace konig {

class ContractAndAnnouncements;
class Player;

class Contract : public boost::enable_shared_from_this<Contract> {
  friend class boost::serialization::access;
  public:
    typedef boost::shared_ptr<Contract> Ptr;

    const std::string& short_name() const { return short_name_; }

    const std::string& name() const { return name_; }

    virtual boost::tuple<Outcome, std::vector<Trick> > play(
        boost::array<Cards, 4> hands,
        boost::array<Cards, 2> talon,
        const std::vector<boost::shared_ptr<Player>>& players,
        PlayPosition declarer_position
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
        const std::vector<boost::shared_ptr<Player>>& players,
        const ContractAndAnnouncements& whole_contract,
        PlayPosition declarer_position,
        bool offence[4]
      );

    static Ptr solodreier();
  protected:
    Contract() = default; // For serialization
    Contract(std::string short_name, std::string name) :
      short_name_(std::move(short_name)),
      name_(std::move(name)) {}

    std::string short_name_;
    std::string name_;
  private:
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(short_name_) &
        BOOST_SERIALIZATION_NVP(name_);
    }
};

}

#endif // KONIG__CONTRACT_HPP

