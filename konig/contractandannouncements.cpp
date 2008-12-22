#include <konig/contractandannouncements.hpp>

#include <boost/foreach.hpp>

namespace konig {

ContractAndAnnouncements::ContractAndAnnouncements(
    Contract::Ptr contract,
    Card called_king
  ) :
  contract_(std::move(contract)),
  called_king_(called_king),
  announcednesses_(
      contract_ ? contract_->initial_announcednesses() : Announcednesses()
    )
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
    announcednesses_.find(std::make_pair(originally_offence, f));
  if (a == announcednesses_.end()) {
    return false;
  }
  // Check it's of the "next level up"
  return (new_announcedness == a->second.next_level());
}

void ContractAndAnnouncements::add(std::vector<Announcement> announcements)
{
  last_announcements_ = std::move(announcements);
  //announcements_.push_back(last_announcements_);
  BOOST_FOREACH(const Announcement& a, last_announcements_) {
    Announcedness new_announcedness = a.announcedness;
    bool offensive = a.offensive();
    Feat f = a.feat;
    announcednesses_[std::make_pair(offensive, f)] = new_announcedness;
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
    const std::vector<Trick>& tricks,
    const Cards& declarers_cards,
    const Cards& defenses_cards,
    bool offence[4]
  )
{
  assert(declarers_cards.size() + defenses_cards.size() == 54);
  Outcome o(contract_);
  BOOST_FOREACH(
      const Announcednesses::value_type& stuff,
      announcednesses_
    ) {
    Achievement a = stuff.first.second.result_for(
        contract_, called_king_, tricks, declarers_cards, defenses_cards,
        stuff.first.first, offence
      );
    o.add(stuff.first.first, stuff.first.second, stuff.second, a);
  }
  return o;
}

}

