#include <konig/negativecontract.hpp>

#include <konig/player.hpp>
#include <konig/announcementsequence.hpp>

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

boost::tuple<Outcome, std::vector<Trick> > NegativeContract::play(
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

