#ifndef KONIG_CONTRACT_HPP
#define KONIG_CONTRACT_HPP

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
#include <konig/playresult.hpp>

namespace konig {

class ContractAndAnnouncements;
class Player;

class KONIG_API Contract : public boost::enable_shared_from_this<Contract> {
  friend class boost::serialization::access;
  public:
    typedef boost::shared_ptr<Contract> Ptr;
    typedef boost::shared_ptr<Contract const> ConstPtr;

    const std::string& bid_name() const { return short_name_; }
    virtual std::string contract_name(
        uint8_t const num_offence,
        Announcedness const
      ) const = 0;
    virtual std::string outcome_name(
        uint8_t const num_achievers,
        Announcedness const,
        Achievement const,
        bool conceded = false
      ) const = 0;

    const std::string& name() const { return name_; }

    virtual PlayResult play(
        std::array<Cards, 4> hands,
        std::array<Cards, 2> talon,
        const std::vector<boost::shared_ptr<Player>>& players,
        PlayPosition declarer_position,
        std::ostream* debug_stream
      ) const = 0;

    virtual bool has_no_announcements() const = 0;

    virtual bool is_partnership() const = 0;

    virtual bool grants_lead() const = 0;

    virtual bool rising_rule() const = 0;

    virtual bool hold_pagat() const = 0;

    virtual bool involves_talon() const = 0;

    virtual bool must_announce_bird() const = 0;

    virtual bool ouvert() const = 0;

    virtual Announcednesses initial_announcednesses() const = 0;

    virtual bool valid_first_announcements(
        const std::vector<Announcement>&
      ) const = 0;

    virtual Achievement result_for(
      const Cards& declarers_cards,
      const std::vector<Trick>& tricks
    ) const = 0;

    virtual int value_of(
      Feat,
      Announcedness,
      Achievement,
      bool against_three,
      Announcednesses const&
    ) const = 0;

    std::vector<Trick> play_tricks(
        std::array<Cards, 4> hands,
        Cards& declarers_cards,
        Cards& defenses_cards,
        const std::vector<boost::shared_ptr<Player>>& players,
        const ContractAndAnnouncements& whole_contract,
        PlayPosition declarer_position,
        std::array<bool, 4> const& offence,
        std::ostream* debug_stream
      ) const;

    // Contracts
    static Ptr rufer();
    static Ptr trischaken();
    static Ptr sechserdreier();
    static Ptr solo();
    static Ptr piccolo();
    static Ptr besserrufer();
    static Ptr bettel();
    static Ptr dreier();
    static Ptr piccolo_ouvert();
    static Ptr besserdreier();
    static Ptr bettel_ouvert();
    static Ptr solodreier();
  protected:
    Contract() {} // For serialization
    Contract(std::string short_name, std::string name) :
      short_name_(std::move(short_name)),
      name_(std::move(name)) {}
  private:
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(short_name_) &
        BOOST_SERIALIZATION_NVP(name_);
    }

    std::string short_name_;
    std::string name_;
};

}

#endif // KONIG_CONTRACT_HPP

