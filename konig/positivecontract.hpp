#ifndef KONIG__POSITIVECONTRACT_HPP
#define KONIG__POSITIVECONTRACT_HPP

#include <vector>

#include <konig/feat.hpp>
#include <konig/contract.hpp>

namespace konig {

class PositiveContract : public Contract {
  public:
    PositiveContract(
        std::string name,
        const bool partnership,
        const uint8_t talon_halves,
        const bool must_announce_bird,
        const bool no_initial_announcements
      );

    virtual Outcome play(
        boost::array<Cards, 4> hands, boost::array<Cards, 2> talon,
        const std::vector<Player::Ptr>& players, PlayPosition declarer_position
      );

    virtual bool grants_lead() const { return false; }

    virtual bool rising_rule() const { return false; }

    virtual Announcednesses initial_announcednesses() const;

    virtual bool valid_first_announcements(
        const std::vector<Announcement>&
      ) const;

    virtual Achievement result_for(const Cards& declarers_cards);
  private:
    bool partnership_;
    uint8_t talon_halves_; // How many halves of the talon declarer gets
    bool must_announce_bird_;
    bool no_initial_announcements_;
};

}

#endif // KONIG__POSITIVECONTRACT_HPP

