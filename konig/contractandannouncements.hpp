#ifndef KONIG__CONTRACTANDANNOUNCEMENTS_HPP
#define KONIG__CONTRACTANDANNOUNCEMENTS_HPP

#include <map>
#include <array>

#include <boost/bind.hpp>

#include <konig/utility/for_all.hpp>
#include <konig/contract.hpp>
#include <konig/playconstraint.hpp>

namespace konig {

class ContractAndAnnouncements {
  public:
    ContractAndAnnouncements(
        Contract::ConstPtr = Contract::ConstPtr(),
        Card called_king = Card(TrumpRank::pagat)
      );

    const Contract::ConstPtr& contract() const { return contract_; }

    const Announcednesses& announcednesses() const { return announcednesses_; }

    const std::vector<Announcement>& last_announcements() const {
      return last_announcements_;
    }

    bool is_legal(const Announcement&, bool offence) const;

    bool are_legal(
        const std::vector<Announcement>& announcements,
        bool first_announcements,
        bool offence
      ) const {
      return utility::for_all(
          announcements,
          boost::bind(&ContractAndAnnouncements::is_legal, this, _1, offence)
        ) &&
        (!first_announcements ||
          contract_->valid_first_announcements(announcements));
    }

    bool is_done() const;

    void set_called_king(const Card king) {
      called_king_ = king;
    }

    void add(std::vector<Announcement>);

    PlayConstraint play_constraint(
        Card,
        bool offence,
        unsigned int trick
      ) const;

    Outcome score(
        const std::vector<Trick>& tricks,
        const Cards& declarers_cards,
        const Cards& defenses_cards,
        std::array<bool, 4> const& game_achievers
      );

    std::string string(uint8_t num_offence) const;
  private:
    Contract::ConstPtr contract_;
    Card called_king_;
    std::vector<Announcement> last_announcements_;
    Announcednesses announcednesses_;
    bool had_first_announcements_;
    uint8_t num_passes_;
    typedef std::map<std::pair<bool, Card>, unsigned int> PlayConstraints;
    PlayConstraints play_constraints_;
};

}

#endif // KONIG__CONTRACTANDANNOUNCEMENTS_HPP

