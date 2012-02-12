#ifndef KONIG_ANNOUNCEMENTSEQUENCE_HPP
#define KONIG_ANNOUNCEMENTSEQUENCE_HPP

#include <boost/tuple/tuple.hpp>
#include <boost/optional.hpp>

#include <konig/player.hpp>
#include <konig/announcement.hpp>
#include <konig/contractandannouncements.hpp>
#include <konig/feat.hpp>

namespace konig {

class AnnouncementSequence {
  public:
    AnnouncementSequence(
      Contract const& contract,
      boost::optional<Card> called_king = boost::optional<Card>()
    ) :
      contract_(contract),
      called_king_(called_king)
    {}

    void get_announcement(
        Player& player,
        bool first_announcements,
        bool offence,
        ContractAndAnnouncements&
      );

    ContractAndAnnouncements
    get_announcements(
      const std::vector<Player::Ptr>&,
      std::array<bool, 4> const& offence,
      PlayPosition declarer_position
    );

    ContractAndAnnouncements
    get_announcements(
      Oracle&,
      PlayPosition declarer_position
    );

    ContractAndAnnouncements
    no_announcements();
  private:
    Contract const& contract_;
    const boost::optional<Card> called_king_;
};

}

#endif // KONIG_ANNOUNCEMENTSEQUENCE_HPP

