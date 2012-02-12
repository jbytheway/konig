#ifndef KONIG_FEAT_HPP
#define KONIG_FEAT_HPP

#include <vector>
#include <array>

#include <boost/shared_ptr.hpp>

#include <konig/card.hpp>
#include <konig/constrainedcard.hpp>
#include <konig/achievement.hpp>
#include <konig/trick.hpp>

namespace konig {

class Contract;

class Feat {
  friend class boost::serialization::access;
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

    Feat() = default; // For serialization
    Feat(internal_enum v) : value_(v) {}
    Feat(TrumpRank);

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

    ConstrainedCard constrained_card(
      boost::optional<Card> const& called_king
    ) const {
      switch (value_) {
        case pagat:
          return ConstrainedCard(Card(TrumpRank::pagat));
        case uhu:
          return ConstrainedCard(Card(TrumpRank::uhu));
        case kakadu:
          return ConstrainedCard(Card(TrumpRank::kakadu));
        case king_ultimo:
          return ConstrainedCard(called_king);
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
        Contract const&,
        boost::optional<Card> const& called_king,
        const std::vector<Trick>& tricks,
        const Cards& declarers_cards,
        const Cards& defences_cards,
        bool feat_offensive,
        std::array<bool, 4> const& achievers
      ) const;
  private:
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(value_);
    }

    internal_enum value_;
};

inline std::ostream& operator<<(std::ostream& o, const Feat f) {
  char name;
  switch (f) {
    case Feat::game:
      return o;
    case Feat::pagat:
      name = '1';
      break;
    case Feat::uhu:
      name = '2';
      break;
    case Feat::kakadu:
      name = '3';
      break;
    case Feat::forty_five:
      name = 'f';
      break;
    case Feat::valat:
      name = 'v';
      break;
    case Feat::king_ultimo:
      name = 'k';
      break;
    default:
      throw std::logic_error("invalid feat");
  }
  return o << name;
}

}

#endif // KONIG_FEAT_HPP

