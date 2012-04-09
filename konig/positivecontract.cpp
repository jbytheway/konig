#include <konig/positivecontract.hpp>

#include <boost/optional.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/export.hpp>

#include <konig/kingcall.hpp>
#include <konig/announcementsequence.hpp>
#include <konig/players.hpp>
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
    const bool no_initial_announcements,
    const bool cancel_kontra_against_three
  ) :
  Contract(std::move(short_name), std::move(name)),
  value_(value),
  off_multiplier_(off_multiplier),
  feat_values_(std::move(feat_values)),
  partnership_(partnership),
  talon_halves_(talon_halves),
  must_announce_bird_(must_announce_bird),
  no_initial_announcements_(no_initial_announcements),
  cancel_kontra_against_three_(cancel_kontra_against_three)
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
    Achievement const achievement,
    bool conceded
  ) const
{
  std::string result = bid_name();
  if (partnership_ && num_achievers == 1) result += "t";
  result += announcedness.string(Achievement::neutral);
  if (achievement == Achievement::off) {
    if (conceded) {
      result += "c";
    } else {
      result += "/";
    }
  }
  return result;
}

PlayResult PositiveContract::play(
  std::array<Cards, 4> hands, std::array<Cards, 2> talon,
  const Players& players, PlayPosition declarer_position,
  std::ostream* debug_stream
) const
{
  Player& declarer = players[declarer_position];
  Cards& declarers_hand = hands[declarer_position];
  std::array<bool, 4> offence = {{false, false, false, false}};
  offence[declarer_position] = true;

  KingCall king(KingCall::invalid);
  boost::optional<Card> called_king;
  bool against_three = false;

  if (partnership_) {
    while (true) {
      king = declarer.call_king();
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
      if (hands[i].count(*called_king)) {
        offence[i] = true;
        break;
      }
    }

    std::for_each(
      players.begin(), players.end(),
      boost::bind(&Player::notify_call_king, _1, king)
    );

    against_three =
      talon[0].count(*called_king) || talon[1].count(*called_king);
  }

  Cards declarers_cards;
  Cards defences_cards;
  Cards rejected_half;
  Cards discard;

  if (talon_halves_ > 0) {
    std::for_each(
        players.begin(), players.end(),
        boost::bind(&Player::notify_talon, _1, boost::cref(talon))
      );

    // Where appropriate, offer the option to concede
    if (against_three) {
      // NB against_three can only be true if it's a partnership contract (see
      // above) and we also know the talon is revealed, so we know it's a
      // contract where concession is allowed.
      bool concession = declarer.choose_concede();
      if (concession) {
        std::for_each(
          players.begin(), players.end(),
          boost::bind(&Player::notify_concede, _1)
        );

        AnnouncementSequence announcements(*this, called_king);
        ContractAndAnnouncements whole_contract =
          announcements.no_announcements();

        Outcome outcome = whole_contract.score_conceded(offence);
        std::array<int, 4> scores = outcome.compute_scores(offence);
        return PlayResult{outcome, {}, {}, {}, scores};
      }
    }

    if (talon_halves_ == 1) {
      uint8_t talon_half;
      while (true) {
        talon_half = declarer.choose_talon_half();
        if (talon_half < 2) {
          break;
        } else {
          std::ostringstream os;
          os << "invalid talon half " << int(talon_half);
          declarer.notify_invalid(os.str());
        }
      }

      std::for_each(
          players.begin(), players.end(),
          boost::bind(&Player::notify_talon_choice, _1, talon_half)
        );

      if (debug_stream) {
        *debug_stream << "chosen talon half is " << talon[talon_half] <<
          std::endl;
      }

      Cards& chosen_half = talon[talon_half];
      rejected_half = talon[!talon_half];
      declarers_hand.insert(chosen_half);
      chosen_half.clear();
      defences_cards.insert(rejected_half);
    } else {
      declarers_hand.insert(talon[0]);
      declarers_hand.insert(talon[1]);
    }

    while (true) {
      discard = declarer.discard();
      // Check conditions on discard
      if (discard.size() != 3U*talon_halves_ ||
          discard.count(SuitRank::king) != 0 ||
          discard.count(TrumpRank::pagat) != 0 ||
          discard.count(TrumpRank::mond) != 0 ||
          discard.count(TrumpRank::skus) != 0 ||
          !declarers_hand.contains(discard) ||
          // This is supposed to check that trumps are discarded only as
          // necessary
          (discard.count(Suit::trumps) &&
            discard.size()-discard.count(Suit::trumps) !=
              declarers_hand.size()-
                declarers_hand.count(Suit::trumps)-
                declarers_hand.count(SuitRank::king))) {
        std::ostringstream os;
        os << "invalid discard";
        declarer.notify_invalid(os.str());
      } else {
        // All conditions OK
        break;
      }
    }

    if (debug_stream) {
      *debug_stream << "discard is " << discard << std::endl;
    }

    declarers_hand.erase(discard);
    declarers_cards.insert(discard);

    Cards trump_discards;
    Cards::iterator end = trump_discards.end();
    BOOST_FOREACH(Card trump_discard, discard.equal_range(Suit::trump)) {
      end = trump_discards.insert(end, trump_discard);
    }

    for (auto& p : players) {
      if (&p == &declarer) {
        p.notify_discard(discard);
      } else {
        p.notify_discard(trump_discards);
      }
    }
  } else {
    // In solo against three, declarer gets the talon
    if (partnership_ && against_three) {
      declarers_cards.insert(talon[0]);
      declarers_cards.insert(talon[1]);
    } else {
      defences_cards.insert(talon[0]);
      defences_cards.insert(talon[1]);
    }
  }

  AnnouncementSequence announcements(*this, called_king);
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
  Outcome outcome = whole_contract.score(
    tricks, declarers_cards, defences_cards, offence, called_king
  );
  std::array<int, 4> scores = outcome.compute_scores(offence);
  return PlayResult{outcome, tricks, rejected_half, discard, scores};
}

PlayResult PositiveContract::play(
  Oracle& oracle,
  PlayPosition declarer_position
) const
{
  std::array<bool, 4> offence = {{false, false, false, false}};
  offence[declarer_position] = true;

  KingCall king(KingCall::invalid);
  boost::optional<Card> called_king;

  if (partnership_) {
    king = oracle.call_king(declarer_position);
    oracle.notify_call_king(king);
    if (king != KingCall::fourth_king) {
      called_king = Card(Suit(king), SuitRank::king);
    }
  }

  Cards declarers_cards;
  Cards defences_cards;
  Cards rejected_half;
  Cards discard;

  if (talon_halves_ > 0) {
    std::array<Cards, 2> talon = oracle.get_talon();
    oracle.notify_talon(talon);
    Cards whole_talon(talon[0]);
    whole_talon.insert(talon[1]);

    static_assert(KingCall::invalid > KingCall::fourth_king,
      "need this condition for the following check to correctly exclude "
      "both invalid and fourth-king");
    if (king < KingCall::fourth_king) {
      called_king = Card(Suit(king), SuitRank::king);
    }

    bool against_three =
      (called_king && whole_talon.count(*called_king)) ||
      (king == KingCall::fourth_king && whole_talon.count(SuitRank::king));

    // Where appropriate, offer the option to concede
    if (against_three) {
      // NB against_three can only be true if it's a partnership contract (see
      // above) and we also know the talon is revealed, so we know it's a
      // contract where concession is allowed.
      bool concession = oracle.choose_concede(declarer_position);
      if (concession) {
        oracle.notify_concede();

        if (!called_king) {
          assert(whole_talon.count(SuitRank::king) == 1);
          called_king = *whole_talon.find(SuitRank::king);
        }

        AnnouncementSequence announcements(*this, *called_king);
        ContractAndAnnouncements whole_contract =
          announcements.no_announcements();

        Outcome outcome = whole_contract.score_conceded(offence);
        std::array<int, 4> scores = outcome.compute_scores(offence);
        return PlayResult{outcome, {}, {}, {}, scores};
      }
    }

    if (talon_halves_ == 1) {
      uint8_t talon_half = oracle.choose_talon_half(declarer_position);
      oracle.notify_talon_choice(talon_half);

      rejected_half = talon[!talon_half];
      defences_cards.insert(rejected_half);
    }

    // This is a bit crazy; when AI is discarding we'll see all the cards;
    // otherwise only trumps.  Either way we're doing the right thing in
    // response.
    Cards discard = oracle.discard(declarer_position);
    oracle.notify_discard(discard);
  }

  AnnouncementSequence announcements(*this);
  ContractAndAnnouncements whole_contract =
    announcements.get_announcements(oracle, declarer_position);

  oracle.notify_announcements_done();

  std::vector<Trick> tricks = play_tricks(
    oracle, declarers_cards, defences_cards,
    whole_contract, called_king, declarer_position, offence
  );
  Outcome outcome = whole_contract.score(
    tricks, declarers_cards, defences_cards, offence, called_king
  );
  std::array<int, 4> scores = outcome.compute_scores(offence);
  return PlayResult{outcome, tricks, rejected_half, discard, scores};
}

Announcednesses PositiveContract::initial_announcednesses() const
{
  Announcednesses result;
  // Note that announcements should be inserted in order for maximum
  // performance
  result.reserve(12);

  // Game
  result.insert(result.end(), Feat::game, true, Announcedness::announced);

  // Birds (offensive and defensive)
  result.insert(result.end(), Feat::pagat, false);
  result.insert(result.end(), Feat::pagat, true);
  result.insert(result.end(), Feat::uhu, false);
  result.insert(result.end(), Feat::uhu, true);
  result.insert(result.end(), Feat::kakadu, false);
  result.insert(result.end(), Feat::kakadu, true);

  // 45 and valat (offensive and defensive)
  result.insert(result.end(), Feat::forty_five, false);
  result.insert(result.end(), Feat::forty_five, true);
  result.insert(result.end(), Feat::valat, false);
  result.insert(result.end(), Feat::valat, true);

  // King ultimo
  if (partnership_) {
    result.insert(result.end(), Feat::king_ultimo, true);
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

int PositiveContract::value_of(
  Feat f,
  Announcedness an,
  Achievement ac,
  bool against_three,
  Announcednesses const& announcednesses
) const
{
  if (f == Feat::game) {
    auto multiplier = an.multiplier();
    // Nasty special case for Solo kontras
    if (cancel_kontra_against_three_ &&
        against_three &&
        ac == Achievement::off &&
        an == Announcedness::kontraed) {
      // We cancel the kontra *unless* declarer made another announcement
      bool cancel = true;
      BOOST_FOREACH(auto const p, announcednesses) {
        Feat const f = p.first.first;
        bool const offence = p.first.second;
        Announcedness a = p.second;
        if (offence && f != Feat::game && a != Announcedness::unannounced) {
          cancel = false;
          break;
        }
      }
      if (cancel) {
        multiplier = 1;
      }
    }
    int value = value_ * multiplier;
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

Achievement PositiveContract::result_for(
  const Cards& declarers_cards,
  const std::vector<Trick>& /*tricks*/
) const
{
  return declarers_cards.total_card_points() >= 36*3-1 ?
    Achievement::made : Achievement::off;
}

}

