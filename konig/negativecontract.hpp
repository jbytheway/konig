#ifndef KONIG_NEGATIVECONTRACT_HPP
#define KONIG_NEGATIVECONTRACT_HPP

#include <vector>

#include <konig/feat.hpp>
#include <konig/contract.hpp>

namespace konig {

class NegativeContract : public Contract {
  friend class boost::serialization::access;
  public:
    NegativeContract(
        std::string short_name,
        std::string name,
        const int value,
        const uint8_t tricks_to_win,
        const bool ouvert,
        const bool grants_lead
      );

    virtual std::string contract_name(
        uint8_t const num_offence,
        Announcedness const
      ) const;
    virtual std::string outcome_name(
        uint8_t const num_achievers,
        Announcedness const,
        Achievement const,
        bool conceded = false
      ) const;

    virtual PlayResult play(
      std::array<Cards, 4> hands,
      std::array<Cards, 2> talon,
      const Players& players,
      PlayPosition declarer_position,
      std::ostream* debug_stream
    ) const;

    virtual PlayResult play(
      Oracle&,
      PlayPosition declarer_position
    ) const;

    virtual bool has_no_announcements() const { return false; }

    virtual bool is_partnership() const { return false; }

    virtual uint8_t talon_halves() const { return 0; }

    virtual bool grants_lead() const { return grants_lead_; }

    virtual bool rising_rule() const { return true; }

    virtual bool hold_pagat() const { return false; }

    virtual bool involves_talon() const { return false; }

    virtual bool must_announce_bird() const { return false; }

    virtual bool ouvert() const { return ouvert_; }

    virtual Announcednesses initial_announcednesses() const;

    virtual bool valid_first_announcements(
        const std::vector<Announcement>&
      ) const;

    virtual int value_of(
      Feat,
      Announcedness,
      Achievement,
      bool against_three,
      Announcednesses const&
    ) const;

    virtual Achievement result_for(
      const Cards& declarers_cards,
      const std::vector<Trick>& tricks
    ) const;
  private:
    NegativeContract() {} // For serialization

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & boost::serialization::make_nvp(
          "base", boost::serialization::base_object<Contract>(*this)
        ) &
        BOOST_SERIALIZATION_NVP(value_) &
        BOOST_SERIALIZATION_NVP(tricks_to_win_) &
        BOOST_SERIALIZATION_NVP(ouvert_) &
        BOOST_SERIALIZATION_NVP(grants_lead_);
    }

    int value_;
    // Declarer must win exactly this many tricks to succeed
    uint8_t tricks_to_win_;
    bool ouvert_;
    bool grants_lead_;
};

}

#endif // KONIG_NEGATIVECONTRACT_HPP

