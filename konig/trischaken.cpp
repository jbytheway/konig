#include <konig/trischaken.hpp>

#include <numeric>

#include <boost/serialization/export.hpp>
#include <boost/algorithm/minmax_element.hpp>
#include <boost/spirit/home/phoenix/operator/logical.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>

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

std::string Trischaken::contract_name(
    uint8_t const /*num_offence*/,
    Announcedness const /*announcedness*/
  ) const
{
  return bid_name();
}

std::string Trischaken::outcome_name(
    uint8_t const num_achievers,
    Announcedness const announcedness,
    Achievement const achievement
  ) const
{
  std::string result = bid_name();
  std::string suffix;
  if (announcedness == Announcedness::kontraed) {
    suffix = "x";
  } else if (achievement == Achievement::made) {
    suffix = "+";
  } else {
    suffix = "";
  }

  switch (num_achievers) {
    case 1:
      break;
    case 2:
      result += "2";
      break;
    case 3:
      result += "3";
      break;
    case 4:
      result += "4";
      break;
    default:
      KONIG_FATAL("invalid num_achievers");
  }
  return result + suffix;
}

PlayResult Trischaken::play(
    std::array<Cards, 4> hands,
    std::array<Cards, 2> /*talon*/,
    const std::vector<boost::shared_ptr<Player>>& players,
    PlayPosition declarer_position,
    std::ostream* debug_stream
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
      players, whole_contract, declarer_position, offence, debug_stream
    );
  // For Trischaken things are complicated, so we have to do some stuff here
  // that would normally be inside Feat
  std::array<bool, 4> achievers;
  std::array<CardPoints, 4> card_points = {{0, 0, 0, 0}};
  BOOST_FOREACH(Trick const& t, tricks) {
    card_points[t.winner()] += Cards(t.cards()).total_card_points();
  }
  // Divide by three (round to nearest) to get 'honest' card points
  BOOST_FOREACH(CardPoints& p, card_points) {
    p = (p+1)/3;
  }
  auto p = boost::minmax_element(card_points.begin(), card_points.end());
  CardPoints min = *p.first, max = *p.second;
  if (min == 0) {
    // Winners are those who took no tricks
    for (PlayPosition p = position_forehand; p != position_max; ++p) {
      achievers[p] = card_points[p] == 0;
    }
  } else {
    // Everyone took a trick, so losers are those who took the most
    for (PlayPosition p = position_forehand; p != position_max; ++p) {
      achievers[p] = card_points[p] == max;
    }
    // When declarer was the only one to take the most, it counts like a kontra
    if (achievers[0] && !achievers[1] && !achievers[2] && !achievers[3]) {
      Announcement an(Feat::game, Announcedness::kontraed, false);
      whole_contract.add(std::vector<Announcement>(1, an));
    }
  }
  Outcome outcome =
    whole_contract.score(tricks, declarers_cards, defences_cards, achievers);
  std::array<int, 4> scores = outcome.compute_scores(achievers);
  return PlayResult{outcome, tricks, scores};
}

Announcednesses Trischaken::initial_announcednesses() const
{
  Announcednesses result;
  result.insert(Feat::game, true, Announcedness::announced);
  return result;
}

bool Trischaken::valid_first_announcements(
    const std::vector<Announcement>& announcements
  ) const
{
  return announcements.empty();
}

int Trischaken::value_of(Feat f, Announcedness an, Achievement ac) const
{
  assert(f == Feat::game);
  assert(ac != Achievement::neutral);
  assert(an <= Announcedness::kontraed);
  int value = an.multiplier();
  if (ac == Achievement::off) value *= -1;
  return value;
}

Achievement Trischaken::result_for(
  const Cards& /*declarers_cards*/,
  const std::vector<Trick>& tricks
) const
{
  // HACK: Have to stupidly repeat the logic above because Trischaken is so
  // differently scored from the other contracts...
  std::array<CardPoints, 4> card_points = {{0, 0, 0, 0}};
  BOOST_FOREACH(Trick const& t, tricks) {
    card_points[t.winner()] += Cards(t.cards()).total_card_points();
  }
  // Divide by three (round to nearest) to get 'honest' card points
  BOOST_FOREACH(CardPoints& p, card_points) {
    p = (p+1)/3;
  }
  CardPoints min  = *std::min_element(card_points.begin(), card_points.end());
  if (min == 0) {
    // Winners are those who took no tricks
    return Achievement::made;
  } else {
    // Everyone took a trick, so losers are those who took the most
    return Achievement::off;
  }
}

}

