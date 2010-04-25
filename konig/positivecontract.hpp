#ifndef KONIG__POSITIVECONTRACT_HPP
#define KONIG__POSITIVECONTRACT_HPP

#include <vector>

#include <konig/feat.hpp>
#include <konig/contract.hpp>

namespace konig {

class PositiveContract : public Contract {
  friend class boost::serialization::access;
  public:
    PositiveContract(
        std::string short_name,
        std::string name,
        const bool partnership,
        const uint8_t talon_halves,
        const bool must_announce_bird,
        const bool no_initial_announcements
      );

    virtual std::string contract_name(
        uint8_t const num_offence,
        Announcedness const
      ) const;
    virtual std::string outcome_name(
        uint8_t const num_achievers,
        Announcedness const,
        Achievement const
      ) const;

    virtual boost::tuple<Outcome, std::vector<Trick> > play(
        std::array<Cards, 4> hands,
        std::array<Cards, 2> talon,
        const std::vector<boost::shared_ptr<Player>>& players,
        PlayPosition declarer_position,
        std::ostream* debug_stream
      ) const;

    virtual bool grants_lead() const { return false; }

    virtual bool rising_rule() const { return false; }

    virtual Announcednesses initial_announcednesses() const;

    virtual bool valid_first_announcements(
        const std::vector<Announcement>&
      ) const;

    virtual Achievement result_for(const Cards& declarers_cards) const;
  private:
    PositiveContract() = default; // For serialization

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & boost::serialization::make_nvp(
          "base", boost::serialization::base_object<Contract>(*this)
        ) &
        BOOST_SERIALIZATION_NVP(partnership_) &
        BOOST_SERIALIZATION_NVP(talon_halves_) &
        BOOST_SERIALIZATION_NVP(must_announce_bird_) &
        BOOST_SERIALIZATION_NVP(no_initial_announcements_);
    }

    bool partnership_;
    uint8_t talon_halves_; // How many halves of the talon declarer gets
    bool must_announce_bird_;
    bool no_initial_announcements_;
};

}

#endif // KONIG__POSITIVECONTRACT_HPP

