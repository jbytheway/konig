#include <konig/contract.hpp>

#include <sstream>

#include <boost/bind.hpp>

#include <konig/fatal.hpp>
#include <konig/player.hpp>
#include <konig/positivecontract.hpp>
#include <konig/negativecontract.hpp>
#include <konig/trischaken.hpp>
#include <konig/contractandannouncements.hpp>

namespace konig {

std::vector<Trick> Contract::play_tricks(
    std::array<Cards, 4> hands,
    Cards& declarers_cards,
    Cards& defences_cards,
    const std::vector<Player::Ptr>& players,
    const ContractAndAnnouncements& whole_contract,
    PlayPosition declarer_position,
    std::array<bool, 4> const& offence,
    std::ostream* debug_stream
  ) const
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
    Trick t(leading, rising_rule(), hold_pagat());
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
        } else {
          std::ostringstream os;
          os << "invalid play " << c << " to trick " << t << " from hand " <<
            hands[playing];
          players[playing]->notify_invalid(os.str());
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
    if (debug_stream) {
      *debug_stream << t << std::endl;
    }
    tricks.push_back(std::move(t));
  }

  return tricks;
}

Contract::Ptr Contract::rufer()
{
  return Ptr(new PositiveContract(
      "r", "rufer", 1, -1, FeatValues::cheltenham(),
      true, 1, false, false, false
    ));
}

Contract::Ptr Contract::trischaken()
{
  return Ptr(new Trischaken());
}

Contract::Ptr Contract::sechserdreier()
{
  return Ptr(new PositiveContract(
      "6d", "sechserdreier", 3, -2, FeatValues::cheltenham(),
      false, 2, false, true, false
    ));
}

Contract::Ptr Contract::solo()
{
  return Ptr(new PositiveContract(
      "s", "solo", 2, -1, FeatValues::cheltenham()*2,
      true, 0, false, false, true
    ));
}

Contract::Ptr Contract::piccolo()
{
  return Ptr(new NegativeContract("p", "piccolo", 2, 1, false, false));
}

Contract::Ptr Contract::besserrufer()
{
  return Ptr(new PositiveContract(
      "br", "besserrufer", 1, -1, FeatValues::cheltenham(),
      true, 1, true, false, false
    ));
}

Contract::Ptr Contract::bettel()
{
  return Ptr(new NegativeContract("b", "bettel", 3, 0, false, true));
}

Contract::Ptr Contract::dreier()
{
  return Ptr(new PositiveContract(
      "d", "dreier", 3, -1, FeatValues::cheltenham(),
      false, 1, false, false, false
    ));
}

Contract::Ptr Contract::piccolo_ouvert()
{
  return Ptr(new NegativeContract("po", "piccolo ouvert", 4, 1, true, false));
}

Contract::Ptr Contract::besserdreier()
{
  return Ptr(new PositiveContract(
      "bd", "besserdreier", 3, -1, FeatValues::cheltenham(),
      false, 1, true, false, false
    ));
}

Contract::Ptr Contract::bettel_ouvert()
{
  return Ptr(new NegativeContract("bo", "bettel ouvert", 6, 0, true, true));
}

Contract::Ptr Contract::solodreier()
{
  return Ptr(new PositiveContract(
      "sd", "solodreier", 6, -1, FeatValues::cheltenham()*2,
      false, 0, false, false, false
    ));
}

}

