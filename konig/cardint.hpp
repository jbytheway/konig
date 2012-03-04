#ifndef KONIG_CARDINT_HPP
#define KONIG_CARDINT_HPP

namespace konig {

// It's handy sometimes to look at cards as integers instead of structs
typedef std::uint32_t CardInt;

static constexpr CardInt card_int_max = 54;

struct IntToCard {
  Card operator()(CardInt const c) const {
    assert(c < card_int_max);
    if (c < 8*4) {
      Suit const s(c/8);
      auto const r = SuitRank::from_value(c%8+SuitRank::min);
      return Card{s, r};
    } else {
      TrumpRank r(c-8*4+TrumpRank::min);
      return Card{r};
    }
  }
};

struct CardToInt {
  CardInt operator()(Card const c) const {
    if (c.trump()) {
      return c.trump_rank()+8*4-TrumpRank::min;
    } else {
      return c.suit()*8+c.suit_rank()-SuitRank::min;
    }
  }
};

typedef utility::SmallSet<CardInt, card_int_max> CardIntSet;

}

#endif // KONIG_CARDINT_HPP

