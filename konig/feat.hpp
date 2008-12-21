#ifndef KONIG__FEAT_HPP
#define KONIG__FEAT_HPP

#include <vector>

#include <boost/shared_ptr.hpp>

#include <konig/card.hpp>
#include <konig/achievement.hpp>
#include <konig/trick.hpp>

namespace konig {

class Contract;

class Feat {
  public:
    enum internal_enum {
      game,
      pagat,
      uhu,
      kakadu,
      forty_five,
      valat,
      king_ultimo
    };

    Feat(internal_enum v) : value_(v) {}

    operator internal_enum() const { return value_; }

    bool is_bird() const {
      return
        value_ == pagat ||
        value_ == uhu ||
        value_ == kakadu;
    }

    bool constrains_play() const {
      return
        value_ == pagat ||
        value_ == uhu ||
        value_ == kakadu ||
        value_ == king_ultimo;
    }

    Card constrained_card(Card called_king) const {
      switch (value_) {
        case pagat:
          return Card(TrumpRank::pagat);
        case uhu:
          return Card(TrumpRank::uhu);
        case kakadu:
          return Card(TrumpRank::kakadu);
        case king_ultimo:
          return called_king;
        default:
          throw std::logic_error("feat does not constrain play");
      }
    }

    unsigned int constrained_to_trick() const {
      switch (value_) {
        case king_ultimo:
        case pagat:
          return 1;
        case uhu:
          return 2;
        case kakadu:
          return 3;
        default:
          throw std::logic_error("feat does not constrain play");
      }
    }

    Achievement result_for(
        const boost::shared_ptr<Contract>&,
        Card called_king,
        const std::vector<Trick>& tricks,
        const Cards& declarers_cards,
        const Cards& defences_cards,
        bool feat_offensive,
        bool offence[4]
      ) const;
  private:
    internal_enum value_;
};

}

#endif // KONIG__FEAT_HPP

