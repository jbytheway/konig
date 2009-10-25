#include <konig/trischaken.hpp>

#include <boost/serialization/export.hpp>

#include <konig/fatal.hpp>
#include <konig/player.hpp>
#include <konig/announcementsequence.hpp>
#include <konig/protocol.hpp> // For achive classes so serialization works

BOOST_CLASS_EXPORT(konig::Trischaken)

namespace konig {

Trischaken::Trischaken() :
  Contract("t", "trischaken")
{
}

boost::tuple<Outcome, std::vector<Trick> > Trischaken::play(
    boost::array<Cards, 4> hands,
    boost::array<Cards, 2> /*talon*/,
    const std::vector<boost::shared_ptr<Player>>& players,
    PlayPosition declarer_position
  ) const
{
  bool offence[4] = {false};
  offence[declarer_position] = true;

  AnnouncementSequence announcements(shared_from_this());
  ContractAndAnnouncements whole_contract =
    announcements.get_announcements(players, offence, declarer_position);

  std::for_each(
      players.begin(), players.end(),
      boost::bind(&Player::notify_announcements_done, _1)
    );

  Cards declarers_cards;
  Cards defences_cards;
  std::vector<Trick> tricks = play_tricks(
      hands, declarers_cards, defences_cards,
      players, whole_contract, declarer_position, offence
    );
  Outcome outcome =
    whole_contract.score(tricks, declarers_cards, defences_cards, offence);
  return boost::make_tuple(outcome, tricks);
}

Announcednesses Trischaken::initial_announcednesses() const
{
  return Announcednesses();
}

bool Trischaken::valid_first_announcements(
    const std::vector<Announcement>& announcements
  ) const
{
  return announcements.empty();
}

Achievement Trischaken::result_for(const Cards& /*declarers_cards*/) const
{
  KONIG_FATAL("interface does not support this yet");
}

}

