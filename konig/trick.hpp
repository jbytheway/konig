#ifndef KONIG__TRICK_HPP
#define KONIG__TRICK_HPP

#include <boost/array.hpp>

#include <konig/playposition.hpp>
#include <konig/cards.hpp>

namespace konig {

class ContractAndAnnouncements;

class Trick {
  public:
    Trick(PlayPosition leader, bool rising_rule) :
      leader_(leader),
      rising_rule_(rising_rule),
      played_(0)
    {}

    bool legal_play(
        Card,
        const Cards& hand,
        bool offence,
        unsigned int trick,
        const ContractAndAnnouncements&
      ) const;

    bool complete() const { return played_ == 4; }

    void add(Card card) {
      assert(!complete()); cards_[played_++] = card;
      if (complete()) determine_winner();
    }

    PlayPosition leader() const { return leader_; }

    const boost::array<Card, 4> cards() const {
      assert(complete()); return cards_;
    }

    PlayPosition winner() const { assert(complete()); return winner_; }

    Card winning_card() const {
      assert(complete()); return cards_[(winner_-leader_+4)%4];
    }

    bool count(const Card& card) const {
      return std::count(cards_.begin(), cards_.begin()+played_, card);
    }

    PlayPosition player_of(const Card& card) const {
      boost::array<Card, 4>::const_iterator p =
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
    boost::array<Card, 4> cards_;
    uint8_t played_;
    PlayPosition winner_;
};

std::ostream& operator<<(std::ostream&, const Trick&);

}

#endif // KONIG__TRICK_HPP

