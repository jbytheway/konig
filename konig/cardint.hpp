#ifndef KONIG_CARDINT_HPP
#define KONIG_CARDINT_HPP

namespace konig {

typedef Card::index_type CardInt;

static constexpr Card::index_type card_int_max = Card::index_max;

struct IntToCard {
  Card operator()(Card::index_type const i) const {
    return Card::from_index(i);
  }
};

struct CardToInt {
  Card::index_type operator()(Card const c) const {
    return c.index();
  }
};

typedef utility::SmallSet<Card::index_type, Card::index_max> CardIntSet;

}

#endif // KONIG_CARDINT_HPP

