#ifndef KONIG_TRISCHAKEN_HPP
#define KONIG_TRISCHAKEN_HPP

#include <vector>

#include <konig/feat.hpp>
#include <konig/contract.hpp>

namespace konig {

class Trischaken : public Contract {
  friend class boost::serialization::access;
  public:
    Trischaken();

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
      const std::vector<boost::shared_ptr<Player>>& players,
      PlayPosition declarer_position,
      std::ostream* debug_stream
    ) const;

    virtual PlayResult play(
      Oracle&,
      PlayPosition declarer_position
    ) const;

    virtual bool has_no_announcements() const { return true; }

    virtual bool is_partnership() const { return false; }

    virtual uint8_t talon_halves() const { return 0; }

    virtual bool grants_lead() const { return false; }

    virtual bool rising_rule() const { return true; }

    virtual bool hold_pagat() const { return true; }

    virtual bool involves_talon() const { return false; }

    virtual bool must_announce_bird() const { return false; }

    virtual bool ouvert() const { return false; }

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
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & boost::serialization::make_nvp(
          "base", boost::serialization::base_object<Contract>(*this)
        );
    }
};

}

#endif // KONIG_TRISCHAKEN_HPP

