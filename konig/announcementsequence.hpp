#ifndef KONIG__ANNOUNCEMENTSEQUENCE_HPP
#define KONIG__ANNOUNCEMENTSEQUENCE_HPP

#include <boost/tuple/tuple.hpp>

#include <konig/player.hpp>
#include <konig/announcement.hpp>
#include <konig/contractandannouncements.hpp>
#include <konig/feat.hpp>

namespace konig {

class AnnouncementSequence {
  public:
    AnnouncementSequence(
        boost::shared_ptr<Contract const> contract,
        Card called_king
      ) :
      contract_(std::move(contract)),
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
        const bool offence[4],
        PlayPosition declarer_position
      );
  private:
    const boost::shared_ptr<Contract const> contract_;
    const Card called_king_;
    std::vector<std::vector<Announcement> > announcements_;
};

}

#endif // KONIG__ANNOUNCEMENTSEQUENCE_HPP

