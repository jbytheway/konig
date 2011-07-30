#ifndef KONIG_TRICK_HPP
#define KONIG_TRICK_HPP

#include <boost/array.hpp>

#include <konig/playposition.hpp>
#include <konig/cards.hpp>

namespace konig {

class ContractAndAnnouncements;

class KONIG_API Trick {
  public:
    Trick(PlayPosition leader, bool rising_rule, bool hold_pagat) :
      leader_(leader),
      rising_rule_(rising_rule),
      hold_pagat_(hold_pagat),
      played_(0)
    {}

    Cards legal_plays(
        const Cards& hand,
        bool offence,
        unsigned int trick,
        const ContractAndAnnouncements&
      ) const;

    bool complete() const { return played_ == 4; }

    void add(Card card);

    PlayPosition leader() const { return leader_; }

    const std::array<Card, 4> cards() const {
      return cards_;
    }

    Cards cards_so_far() const {
      return Cards(cards_.begin(), cards_.begin() + played_);
    }

    Suit suit() const { assert(played_); return cards_[0].suit(); }

    uint8_t played() const { return played_; }

    PlayPosition winner() const {
      assert(played_);
      return PlayPosition((leader_+winning_card_)%4);
    }

    Card winning_card() const {
      assert(played_); return cards_[winning_card_];
    }

    bool count(const Card& card) const {
      return std::count(cards_.begin(), cards_.begin()+played_, card);
    }

    PlayPosition player_of(const Card& card) const {
      std::array<Card, 4>::const_iterator p =
        std::find(cards_.begin(), cards_.begin()+played_, card);
      if (p == cards_.begin()+played_) {
        return position_max;
      }
      return PlayPosition((leader_+(p-cards_.begin()))%4);
    }
  private:
    void determine_winner();

    PlayPosition leader_;
    bool rising_rule_;
    bool hold_pagat_;
    std::array<Card, 4> cards_;
    uint8_t played_;
    uint8_t winning_card_;
};

KONIG_API std::ostream& operator<<(std::ostream&, const Trick&);

}

#endif // KONIG_TRICK_HPP

