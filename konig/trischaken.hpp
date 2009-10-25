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

    virtual boost::tuple<Outcome, std::vector<Trick> > play(
        boost::array<Cards, 4> hands,
        boost::array<Cards, 2> talon,
        const std::vector<boost::shared_ptr<Player>>& players,
        PlayPosition declarer_position
      ) const;

    virtual bool grants_lead() const { return false; }

    virtual bool rising_rule() const { return true; }

    virtual Announcednesses initial_announcednesses() const;

    virtual bool valid_first_announcements(
        const std::vector<Announcement>&
      ) const;

    virtual Achievement result_for(const Cards& declarers_cards) const;
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

