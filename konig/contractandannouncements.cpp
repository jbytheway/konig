#include <konig/contractandannouncements.hpp>

#include <sstream>

#include <boost/foreach.hpp>

namespace konig {

ContractAndAnnouncements::ContractAndAnnouncements(
  Contract const& contract,
  boost::optional<Card> called_king
) :
  contract_(contract),
  called_king_(called_king),
  announcednesses_(contract_.initial_announcednesses()),
  had_first_announcements_(false),
  num_passes_(0)
{}

bool ContractAndAnnouncements::is_legal(
    const Announcement& announcement,
    bool offence
  ) const
{
  Feat f = announcement.feat;
  Announcedness new_announcedness = announcement.announcedness;
  bool originally_offence = offence ^ new_announcedness.inverted_side();
  // Check it's being said by the right side
  if (originally_offence != announcement.offensive()) {
    return false;
  }
  // Check it exists in this contract
  Announcednesses::const_iterator a =
    announcednesses_.find(std::make_pair(f, originally_offence));
  if (a == announcednesses_.end()) {
    return false;
  }
  // Check it's of the "next level up"
  return (new_announcedness == a->second.next_level());
}

bool ContractAndAnnouncements::is_done() const
{
  return contract_.has_no_announcements() || num_passes_ == 3;
}

void ContractAndAnnouncements::add(std::vector<Announcement> announcements)
{
  last_announcements_ = std::move(announcements);
  if (!had_first_announcements_) {
    had_first_announcements_ = true;
  } else if (last_announcements_.empty()) {
    assert(num_passes_ < 3);
    ++num_passes_;
  } else {
    num_passes_ = 0;
  }

  BOOST_FOREACH(const Announcement& a, last_announcements_) {
    Announcedness new_announcedness = a.announcedness;
    bool offensive = a.offensive();
    Feat f = a.feat;
    announcednesses_[std::make_pair(f, offensive)] = new_announcedness;
    if (new_announcedness == Announcedness::announced &&
        f.constrains_play()) {
      play_constraints_[
        std::make_pair(offensive, f.constrained_card(called_king_))
      ] = f.constrained_to_trick();
    }
  }
}

PlayConstraint ContractAndAnnouncements::play_constraint(
    Card card,
    bool offence,
    unsigned int trick
  ) const
{
  PlayConstraints::const_iterator constraint =
    play_constraints_.find(std::make_pair(offence, card));
  if (constraint != play_constraints_.end()) {
    if (trick > constraint->second) {
      return PlayConstraint::avoid_playing;
    } else if (trick == constraint->second) {
      return PlayConstraint::try_to_play;
    } else {
      return PlayConstraint::neutral;
    }
  }
  return PlayConstraint::neutral;
}

Outcome ContractAndAnnouncements::score(
    std::vector<Trick> const& tricks,
    Cards const& declarers_cards,
    Cards const& defenses_cards,
    std::array<bool, 4> const& game_achievers,
    boost::optional<Card> const& called_king
  )
{
#ifndef NDEBUG
  if (contract_.involves_talon()) {
    assert(declarers_cards.size() + defenses_cards.size() == 54);
  } else {
    assert(declarers_cards.size() + defenses_cards.size() == 48);
  }
  if (called_king_) {
    assert(called_king);
    assert(*called_king == *called_king_);
  } else if (contract_.is_partnership()) {
    assert(called_king);
  }
#endif
  uint8_t const num_game_achievers =
    std::count(game_achievers.begin(), game_achievers.end(), true);
  bool const against_three = (num_game_achievers == 1);
  Outcome o(contract_, num_game_achievers, false);
  BOOST_FOREACH(const auto& i, announcednesses_) {
    bool offensive = i.first.second;
    Feat feat = i.first.first;
    Announcedness announcedness = i.second;
    Achievement achievement = feat.result_for(
      contract_, called_king, tricks, declarers_cards, defenses_cards,
      offensive, game_achievers
    );
    o.add(
      offensive, feat, announcedness, achievement, against_three,
      announcednesses_
    );
  }
  return o;
}

Outcome ContractAndAnnouncements::score_conceded(
    std::array<bool, 4> const& game_achievers
  )
{
  uint8_t const num_game_achievers =
    std::count(game_achievers.begin(), game_achievers.end(), true);
  bool const against_three = (num_game_achievers == 1);
  Outcome o(contract_, num_game_achievers, true);
  o.add(
    true /*offensive*/, Feat::game, Announcedness::announced, Achievement::off,
    against_three, announcednesses_
  );
  return o;
}

std::string ContractAndAnnouncements::string(uint8_t num_offence) const
{
  std::ostringstream os;
  if (!announcednesses_.count(std::make_pair(Feat::game, true))) {
    // special case for Trishacken
    os << contract().contract_name(num_offence, Announcedness::announced);
  } else {
    BOOST_FOREACH(auto const& i, announcednesses()) {
      bool defensive = !i.first.second;
      Feat f = i.first.first;
      Announcedness announcedness = i.second;
      if (f == Feat::game) {
        os << contract().contract_name(num_offence, announcedness);
      } else if (announcedness != Announcedness::unannounced) {
        os << f;
        if (defensive) os << "!";
        os << announcedness.string(Achievement::neutral);
      }
    }
  }
  return os.str();
}

}

