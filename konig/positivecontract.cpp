#include <konig/positivecontract.hpp>

#include <boost/bind.hpp>
#include <boost/serialization/export.hpp>

#include <konig/kingcall.hpp>
#include <konig/announcementsequence.hpp>
#include <konig/protocol.hpp> // For achive classes so serialization works

BOOST_CLASS_EXPORT(konig::PositiveContract)

namespace konig {

PositiveContract::PositiveContract(
    std::string short_name,
    std::string name,
    const int value,
    const int off_multiplier,
    FeatValues feat_values,
    const bool partnership,
    const uint8_t talon_halves,
    const bool must_announce_bird,
    const bool no_initial_announcements
  ) :
  Contract(std::move(short_name), std::move(name)),
  value_(value),
  off_multiplier_(off_multiplier),
  feat_values_(std::move(feat_values)),
  partnership_(partnership),
  talon_halves_(talon_halves),
  must_announce_bird_(must_announce_bird),
  no_initial_announcements_(no_initial_announcements)
{
}

std::string PositiveContract::contract_name(
    uint8_t const num_offence,
    Announcedness const announcedness
  ) const
{
  std::string result = bid_name();
  if (partnership_ && num_offence == 1) result += "3";
  result += announcedness.string(Achievement::neutral);
  return result;
}

std::string PositiveContract::outcome_name(
    uint8_t const num_achievers,
    Announcedness const announcedness,
    Achievement const achievement
  ) const
{
  std::string result = bid_name();
  if (partnership_ && num_achievers == 1) result += "3";
  result += announcedness.string(Achievement::neutral);
  if (achievement == Achievement::off) result += "/";
  return result;
}

PlayResult PositiveContract::play(
    std::array<Cards, 4> hands, std::array<Cards, 2> talon,
    const std::vector<Player::Ptr>& players, PlayPosition declarer_position,
    std::ostream* debug_stream
  ) const
{
  Player::Ptr declarer = players[declarer_position];
  Cards& declarers_hand = hands[declarer_position];
  std::array<bool, 4> offence = {{false, false, false, false}};
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

    if (debug_stream) {
      *debug_stream << "called king is " << king << std::endl;
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

    BOOST_FOREACH(Player::Ptr const& p, players) {
      if (p == declarer) {
        p->notify_discard(discard);
      } else {
        p->notify_discard(trump_discards);
      }
    }
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

  std::for_each(
      players.begin(), players.end(),
      boost::bind(&Player::notify_announcements_done, _1)
    );

  std::vector<Trick> tricks = play_tricks(
      hands, declarers_cards, defences_cards,
      players, whole_contract, declarer_position, offence, debug_stream
    );
  Outcome outcome =
    whole_contract.score(tricks, declarers_cards, defences_cards, offence);
  std::array<int, 4> scores = outcome.compute_scores(offence);
  return PlayResult{outcome, tricks, scores};
}

Announcednesses PositiveContract::initial_announcednesses() const
{
  Announcednesses result;
  // Game
  result.insert(Feat::game, true, Announcedness::announced);

  // Birds (offensive and defensive)
  result.insert(Feat::pagat, true);
  result.insert(Feat::uhu, true);
  result.insert(Feat::kakadu, true);
  result.insert(Feat::pagat, false);
  result.insert(Feat::uhu, false);
  result.insert(Feat::kakadu, false);

  // 45 and valat (offensive and defensive)
  result.insert(Feat::forty_five, true);
  result.insert(Feat::valat, true);
  result.insert(Feat::forty_five, false);
  result.insert(Feat::valat, false);

  // King ultimo
  if (partnership_) {
    result.insert(Feat::king_ultimo, true);
  }

  return result;
}

bool PositiveContract::valid_first_announcements(
    const std::vector<Announcement>& announcements
  ) const
{
  if (no_initial_announcements_ && !announcements.empty()) {
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

int PositiveContract::value_of(Feat f, Announcedness an, Achievement ac) const
{
  if (f == Feat::game) {
    int value = value_ * an.multiplier();
    if (ac == Achievement::off) value *= off_multiplier_;
    return value;
  }

  // Non-game feats
  int value = feat_values_.value_of(f, an != Announcedness::unannounced);
  if (an != Announcedness::unannounced) {
    value *= an.multiplier();
  }
  if (ac == Achievement::off) value *= -1;
  return value;
}

Achievement PositiveContract::result_for(const Cards& declarers_cards) const
{
  return declarers_cards.total_card_points() >= 36*3-1 ?
    Achievement::made : Achievement::off;
}

}

