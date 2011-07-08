#include <konig/announcementsequence.hpp>

#include <sstream>

#include <boost/bind.hpp>
#include <boost/range/algorithm/copy.hpp>

namespace konig {

void AnnouncementSequence::get_announcement(
    Player& player,
    bool first_announcements,
    bool offence,
    ContractAndAnnouncements& whole_contract
  )
{
  std::vector<Announcement> announcements;
  while(true) {
    announcements = player.announce();
    if (whole_contract.are_legal(
        announcements, first_announcements, offence)) {
      break;
    }
    std::ostringstream os;
    os << "invalid announcements '";
    boost::range::copy(
      announcements, std::ostream_iterator<Announcement>(os, ", ")
    );
    os << "' in response to " << whole_contract.string(2);
    if (first_announcements) {
      os << " (first)";
    } else if (offence) {
      os << " (offence)";
    } else {
      os << " (defence)";
    }
    player.notify_invalid_announcements(os.str());
  }

  whole_contract.add(announcements);
}

ContractAndAnnouncements
AnnouncementSequence::get_announcements(
    const std::vector<Player::Ptr>& players,
    std::array<bool, 4> const& offence,
    PlayPosition declarer_position
  )
{
  assert(players.size() == 4);
  announcements_.clear();
  ContractAndAnnouncements result(contract_, called_king_);
  PlayPosition announcer = declarer_position;
  // first announcements may be unusual (e.g. forced to announce a bird)
  get_announcement(*players[announcer], true, offence[announcer], result);

  announcements_.push_back(result.last_announcements());
  std::for_each(
      players.begin(), players.end(),
      boost::bind(
        &Player::notify_announcements, _1, result.last_announcements()
      )
    );

  size_t num_passes = 0;
  while (num_passes < 3) {
    ++announcer;
    announcer = PlayPosition(announcer % 4);
    get_announcement(*players[announcer], false, offence[announcer], result);
    announcements_.push_back(result.last_announcements());
    if (result.last_announcements().empty()) {
      ++num_passes;
    } else {
      num_passes = 0;
    }

    std::for_each(
        players.begin(), players.end(),
        boost::bind(
          &Player::notify_announcements, _1, result.last_announcements()
        )
      );
  }

  return result;
}

}

