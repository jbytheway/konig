#include <konig/contract.hpp>

#include <sstream>

#include <boost/bind.hpp>
#include <boost/range/numeric.hpp>

#include <konig/fatal.hpp>
#include <konig/players.hpp>
#include <konig/positivecontract.hpp>
#include <konig/negativecontract.hpp>
#include <konig/trischaken.hpp>
#include <konig/contractandannouncements.hpp>

namespace konig {

std::vector<Trick> Contract::play_tricks(
  std::array<Cards, 4> hands,
  Cards& declarers_cards,
  Cards& defences_cards,
  const Players& players,
  const ContractAndAnnouncements& whole_contract,
  PlayPosition declarer_position,
  std::array<bool, 4> const& offence,
  std::ostream* debug_stream
) const
{
  for (size_t i=0; i<4; ++i) {
    assert(hands[i].size() == 12);
  }
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
        c = players[playing].play_card();
        if (hands[playing].count(c) &&
            t.legal_plays(
              hands[playing], offence[playing], trick_number, whole_contract
            ).count(c)) {
          break;
        } else {
          std::ostringstream os;
          os << "invalid play " << c << " to trick " << t << " from hand " <<
            hands[playing];
          players[playing].notify_invalid(os.str());
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
    if (trick_number == 12 && ouvert()) {
      std::for_each(
        players.begin(), players.end(),
        boost::bind(&Player::notify_ouvert, _1, hands[declarer_position])
      );
    }
  }

  return tricks;
}

std::vector<Trick> Contract::play_tricks(
  Oracle& oracle,
  Cards& declarers_cards,
  Cards& defenses_cards,
  const ContractAndAnnouncements& whole_contract,
  boost::optional<Card>& called_king,
  PlayPosition declarer_position,
  std::array<bool, 4>& offence
) const
{
  assert(declarer_position < 4);
  std::array<Cards, 4> cards_taken;

  PlayPosition leading = position_forehand;
  if (grants_lead()) {
    leading = declarer_position;
  }

  std::vector<Trick> tricks;

  for (unsigned int trick_number = 12; trick_number > 0; --trick_number) {
    Trick t(leading, rising_rule(), hold_pagat());
    for (unsigned int i=0; i<4; ++i) {
      PlayPosition playing = PlayPosition((leading + i) % 4);
      Card c = oracle.play_card(playing);
      // Pick up partner
      if (is_partnership()) {
        if (!called_king) {
          if (playing != declarer_position &&
            !c.trump() && c.suit_rank() == SuitRank::king) {
            called_king = c;
          }
        }

        if (called_king && *called_king == c) {
          offence[playing] = true;
        }
      }
      t.add(c);
      oracle.notify_play_card(playing, c);
    }
    leading = t.winner();
    cards_taken[leading].insert(t.cards());

    tricks.push_back(std::move(t));
    if (trick_number == 12 && ouvert()) {
      Cards ouvert = oracle.get_ouvert(declarer_position);
      oracle.notify_ouvert(ouvert);
    }
  }

  for (size_t i=0; i<4; ++i) {
    if (offence[i]) {
      declarers_cards.insert(cards_taken[i]);
    } else {
      defenses_cards.insert(cards_taken[i]);
    }
  }

  if (involves_talon()) {
    // To score correctly we need to deal with the cards out of play, and who
    // they belong to
    if (talon_halves() > 0) {
      // We need to figure out what declarer's discard was and add it to his
      // cards
      Cards discard = Cards::make_deck();
      discard.erase(declarers_cards);
      discard.erase(defenses_cards);
      assert(discard.size() == talon_halves()*3);
      declarers_cards.insert(discard);
    } else {
      // This is solo(dreier) and we never saw the talon
      Cards talon = Cards::make_deck();
      talon.erase(declarers_cards);
      talon.erase(defenses_cards);
      assert(talon.size() == 6);

      // In solo against three declarer gets the cards; otherwise defenders
      if (is_partnership() && 1 == boost::accumulate(offence, 0)) {
        declarers_cards.insert(talon);
      } else {
        defenses_cards.insert(talon);
      }
    }
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

