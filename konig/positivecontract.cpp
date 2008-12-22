#include <konig/positivecontract.hpp>

#include <boost/bind.hpp>

#include <konig/kingcall.hpp>
#include <konig/announcementsequence.hpp>

namespace konig {

PositiveContract::PositiveContract(
    std::string short_name,
    std::string name,
    const bool partnership,
    const uint8_t talon_halves,
    const bool must_announce_bird,
    const bool no_initial_announcements
  ) :
  Contract(std::move(short_name), std::move(name)),
  partnership_(partnership),
  talon_halves_(talon_halves),
  must_announce_bird_(must_announce_bird),
  no_initial_announcements_(no_initial_announcements)
{
}

boost::tuple<Outcome, std::vector<Trick> > PositiveContract::play(
    boost::array<Cards, 4> hands, boost::array<Cards, 2> talon,
    const std::vector<Player::Ptr>& players, PlayPosition declarer_position
  )
{
  Player::Ptr declarer = players[declarer_position];
  Cards& declarers_hand = hands[declarer_position];
  bool offence[4] = {false};
  offence[declarer_position] = true;

  KingCall king(KingCall::invalid);
  Card called_king(TrumpRank::pagat);
  if (partnership_) {
    while (true) {
      king = declarer->call_king();
      if (king != KingCall::fourth_king) {
        called_king = Card(Suit(king), SuitRank::king);
        break;
      }
      if (declarers_hand.count(SuitRank::king) == 3) {
        for (Suit s = Suit::min; s<Suit::trumps; ++s) {
          if (!declarers_hand.count(Card(s, SuitRank::king))) {
            called_king = Card(s, SuitRank::king);
            break;
          }
        }
        break;
      }
      // In this case declarer called the fourth king while holding all four,
      // which is illegal, so ask again
    }

    // Identify partner
    for (uint8_t i=0; i<4; ++i) {
      if (hands[i].count(called_king)) {
        offence[i] = true;
        break;
      }
    }

    std::for_each(
        players.begin(), players.end(),
        boost::bind(&Player::notify_call_king, _1, king)
      );
  }

  Cards declarers_cards;
  Cards defences_cards;

  if (talon_halves_ > 0) {
    std::for_each(
        players.begin(), players.end(),
        boost::bind(&Player::notify_talon, _1, boost::cref(talon))
      );

    if (talon_halves_ == 1) {
      uint8_t talon_half;
      while (true) {
        talon_half = declarer->choose_talon_half();
        if (talon_half < 2) {
          break;
        }
      }

      std::for_each(
          players.begin(), players.end(),
          boost::bind(&Player::notify_talon_choice, _1, talon_half)
        );

      Cards& chosen_half = talon[talon_half];
      Cards& rejected_half = talon[!talon_half];
      declarers_hand.insert(chosen_half);
      chosen_half.clear();
      defences_cards.insert(rejected_half);
    } else {
      declarers_hand.insert(talon[0]);
      declarers_hand.insert(talon[1]);
    }

    Cards discard;

    while (true) {
      discard = declarer->discard();
      // Check conditions on discard
      if (discard.size() != 3U*talon_halves_ ||
          discard.count(SuitRank::king) != 0 ||
          discard.count(TrumpRank::pagat) != 0 ||
          discard.count(TrumpRank::mond) != 0 ||
          discard.count(TrumpRank::skus) != 0 ||
          !declarers_hand.contains(discard))
        continue;
      // This is supposed to check that trumps are discarded only as necessary
      if (discard.count(Suit::trumps) &&
          discard.size()-discard.count(Suit::trumps) !=
            declarers_hand.size()-
              declarers_hand.count(Suit::trumps)-
              declarers_hand.count(SuitRank::king))
        continue;
      // All conditions OK
      break;
    }

    declarers_hand.erase(discard);
    declarers_cards.insert(discard);

    Cards trump_discards;
    Cards::iterator end = trump_discards.end();
    BOOST_FOREACH(Card trump_discard, discard.equal_range(Suit::trump)) {
      end = trump_discards.insert(end, trump_discard);
    }

    std::for_each(
        players.begin(), players.end(),
        boost::bind(&Player::notify_discard, _1, boost::cref(trump_discards))
      );
  } else {
    // In solo against three, declarer gets the talon
    if (partnership_ &&
        talon[0].count(called_king) + talon[1].count(called_king)) {
      declarers_cards.insert(talon[0]);
      declarers_cards.insert(talon[1]);
    } else {
      defences_cards.insert(talon[0]);
      defences_cards.insert(talon[1]);
    }
  }

  AnnouncementSequence announcements(shared_from_this(), called_king);
  ContractAndAnnouncements whole_contract =
    announcements.get_announcements(players, offence, declarer_position);

  std::vector<Trick> tricks = play_tricks(
      hands, declarers_cards, defences_cards,
      players, whole_contract, declarer_position, offence
    );
  Outcome outcome =
    whole_contract.score(tricks, declarers_cards, defences_cards, offence);
  return boost::make_tuple(outcome, tricks);
}

Announcednesses PositiveContract::initial_announcednesses() const
{
  Announcednesses result;
  // Game
  result.insert(true, Feat::game, Announcedness::announced);

  // Birds (offensive and defensive)
  result.insert(true, Feat::pagat);
  result.insert(true, Feat::uhu);
  result.insert(true, Feat::kakadu);
  result.insert(false, Feat::pagat);
  result.insert(false, Feat::uhu);
  result.insert(false, Feat::kakadu);

  // 45 and valat (offensive and defensive)
  result.insert(true, Feat::forty_five);
  result.insert(true, Feat::valat);
  result.insert(false, Feat::forty_five);
  result.insert(false, Feat::valat);

  // King ultimo
  if (partnership_) {
    result.insert(true, Feat::king_ultimo);
  }

  return result;
}

bool PositiveContract::valid_first_announcements(
    const std::vector<Announcement>& announcements
  ) const
{
  if (no_initial_announcements_ && ! announcements.empty()) {
    return false;
  }
  if (must_announce_bird_) {
    BOOST_FOREACH(const Announcement& announcement, announcements) {
      if (announcement.feat.is_bird()) {
        return true;
      }
    }
    return false;
  }
  return true;
}

Achievement PositiveContract::result_for(const Cards& declarers_cards)
{
  return declarers_cards.total_card_points() >= 36*3-1 ?
    Achievement::made : Achievement::off;
}

}

