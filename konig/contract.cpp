#include <konig/contract.hpp>

#include <boost/bind.hpp>

#include <konig/positivecontract.hpp>
#include <konig/contractandannouncements.hpp>

namespace konig {

std::vector<Trick> Contract::play_tricks(
    boost::array<Cards, 4> hands,
    Cards& declarers_cards,
    Cards& defences_cards,
    const std::vector<Player::Ptr>& players,
    const ContractAndAnnouncements& whole_contract,
    PlayPosition declarer_position,
    bool offence[4]
  )
{
  for (size_t i=0; i<4; ++i) {
    assert(hands[i].size() == 12);
  }
  assert(players.size() == 4);
  assert(declarer_position < 4);

  PlayPosition leading = position_forehand;
  if (grants_lead()) {
    leading = declarer_position;
  }

  std::vector<Trick> tricks;

  for (unsigned int trick_number = 12; trick_number > 0; --trick_number) {
    Trick t(leading, rising_rule());
    for (unsigned int i=0; i<4; ++i) {
      PlayPosition playing = PlayPosition((leading + i) % 4);
      Card c;
      while (true) {
        c = players[playing]->play_card();
        if (hands[playing].count(c) &&
            t.legal_plays(
              hands[playing], offence[playing], trick_number, whole_contract
            ).count(c)) {
          break;
        }
      }
      t.add(c);
      hands[playing].erase(c);
      std::for_each(
          players.begin(), players.end(),
          boost::bind(&Player::notify_play_card, _1, playing, c)
        );
    }
    leading = t.winner();
    if (offence[leading]) {
      declarers_cards.insert(t.cards());
    } else {
      defences_cards.insert(t.cards());
    }
    tricks.push_back(std::move(t));
  }

  return std::move(tricks);
}

Contract::Ptr Contract::solodreier()
{
  return Ptr(new PositiveContract("sd", "solodreier", false, 0, false, false));
}

}

