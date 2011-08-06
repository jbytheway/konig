#ifndef KONIG_CONSTRAINEDCARD_HPP
#define KONIG_CONSTRAINEDCARD_HPP

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include <konig/card.hpp>

namespace konig {

class ConstrainedCard {
  public:
    explicit ConstrainedCard(Card);
    explicit ConstrainedCard(boost::optional<Card> const& called_king);

    Card get_card() const;

    friend bool operator<(ConstrainedCard const&, ConstrainedCard const&);

    struct FourthKingPlaceholder {};
  private:
    friend struct compare_visitor_1;
    typedef boost::variant<Card, FourthKingPlaceholder> Impl;
    Impl card_;
};

}

#endif // KONIG_CONSTRAINEDCARD_HPP

