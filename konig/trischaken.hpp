#ifndef KONIG__TRISCHAKEN_HPP
#define KONIG__TRISCHAKEN_HPP

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
        Achievement const
      ) const;

    virtual PlayResult play(
        std::array<Cards, 4> hands,
        std::array<Cards, 2> talon,
        const std::vector<boost::shared_ptr<Player>>& players,
        PlayPosition declarer_position,
        std::ostream* debug_stream
      ) const;

    virtual bool is_partnership() const { return false; }

    virtual bool grants_lead() const { return false; }

    virtual bool rising_rule() const { return true; }

    virtual bool hold_pagat() const { return true; }

    virtual bool involves_talon() const { return false; }

    virtual bool must_announce_bird() const { return false; }

    virtual Announcednesses initial_announcednesses() const;

    virtual bool valid_first_announcements(
        const std::vector<Announcement>&
      ) const;

    virtual int value_of(Feat, Announcedness, Achievement) const;

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

#endif // KONIG__TRISCHAKEN_HPP

