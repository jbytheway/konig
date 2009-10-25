#include <konig/negativecontract.hpp>

#include <boost/serialization/export.hpp>

#include <konig/player.hpp>
#include <konig/announcementsequence.hpp>
#include <konig/protocol.hpp> // For achive classes so serialization works

BOOST_CLASS_EXPORT(konig::NegativeContract)

namespace konig {

NegativeContract::NegativeContract(
    std::string short_name,
    std::string name,
    const uint8_t tricks_to_win,
    const bool ouvert,
    const bool grants_lead
  ) :
  Contract(std::move(short_name), std::move(name)),
  tricks_to_win_(tricks_to_win),
  ouvert_(ouvert),
  grants_lead_(grants_lead)
{
}

std::string NegativeContract::contract_name(
    uint8_t const /*num_offence*/,
    Announcedness const announcedness
  ) const
{
  std::string result = bid_name();
  result += announcedness.string(Achievement::neutral);
  return result;
}

std::string NegativeContract::outcome_name(
    uint8_t const /*num_achievers*/,
    Announcedness const announcedness,
    Achievement const achievement
  ) const
{
  std::string result = bid_name();
  result += announcedness.string(Achievement::neutral);
  if (achievement == Achievement::off) result += "/";
  return result;
}

boost::tuple<Outcome, std::vector<Trick> > NegativeContract::play(
    std::array<Cards, 4> hands,
    std::array<Cards, 2> /*talon*/,
    const std::vector<boost::shared_ptr<Player>>& players,
    PlayPosition declarer_position
  ) const
{
  std::array<bool, 4> offence = {{false, false, false, false}};
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

Announcednesses NegativeContract::initial_announcednesses() const
{
  Announcednesses result;
  // Game
  result.insert(Feat::game, true, Announcedness::announced);
  return result;
}

bool NegativeContract::valid_first_announcements(
    const std::vector<Announcement>& announcements
  ) const
{
  return announcements.empty();
}

Achievement NegativeContract::result_for(const Cards& declarers_cards) const
{
  assert(declarers_cards.size() % 4 == 0);
  uint8_t declarers_tricks = declarers_cards.size() / 4;
  return declarers_tricks == tricks_to_win_ ?
    Achievement::made : Achievement::off;
}

}

