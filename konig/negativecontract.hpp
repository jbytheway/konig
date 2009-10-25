#ifndef KONIG__NEGATIVECONTRACT_HPP
#define KONIG__NEGATIVECONTRACT_HPP

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
        Achievement const
      ) const;

    virtual boost::tuple<Outcome, std::vector<Trick> > play(
        boost::array<Cards, 4> hands,
        boost::array<Cards, 2> talon,
        const std::vector<boost::shared_ptr<Player>>& players,
        PlayPosition declarer_position
      ) const;

    virtual bool grants_lead() const { return grants_lead_; }

    virtual bool rising_rule() const { return true; }

    virtual Announcednesses initial_announcednesses() const;

    virtual bool valid_first_announcements(
        const std::vector<Announcement>&
      ) const;

    virtual Achievement result_for(const Cards& declarers_cards) const;
  private:
    NegativeContract() = default; // For serialization

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & boost::serialization::make_nvp(
          "base", boost::serialization::base_object<Contract>(*this)
        ) &
        BOOST_SERIALIZATION_NVP(tricks_to_win_) &
        BOOST_SERIALIZATION_NVP(ouvert_) &
        BOOST_SERIALIZATION_NVP(grants_lead_);
    }

    // Declarer must win exactly this many tricks to succeed
    uint8_t tricks_to_win_;
    bool ouvert_;
    bool grants_lead_;
};

}

#endif // KONIG__NEGATIVECONTRACT_HPP

