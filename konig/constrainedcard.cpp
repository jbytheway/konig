#include <konig/constrainedcard.hpp>

namespace konig {

namespace {

  struct get_card_visitor : boost::static_visitor<Card> {
    Card operator()(Card const& c) const {
      return c;
    }

    Card operator()(ConstrainedCard::FourthKingPlaceholder const) const {
      KONIG_FATAL("tried to extract unknown card");
    }
  };

  struct compare_visitor_2 : boost::static_visitor<bool> {
    compare_visitor_2(Card const& r) : r_(r) {}

    bool operator()(Card const& l) const {
      return l < r_;
    }

    bool operator()(ConstrainedCard::FourthKingPlaceholder const) const {
      return true;
    }

    Card const& r_;
  };

}

// Morally this should be in the anonymous namespace but then I can't make it a
// friend of ConstrainedCard.
  struct compare_visitor_1 : boost::static_visitor<bool> {
    compare_visitor_1(ConstrainedCard const& l) : l_(l) {}

    bool operator()(Card const& r) const {
      return boost::apply_visitor(compare_visitor_2(r), l_.card_);
    }

    bool operator()(ConstrainedCard::FourthKingPlaceholder const) const {
      return false;
    }

    ConstrainedCard const& l_;
  };

ConstrainedCard::ConstrainedCard(Card c) :
  card_(std::move(c))
{}

ConstrainedCard::ConstrainedCard(boost::optional<Card> const& c) :
  card_(c ? Impl(*c) : Impl(FourthKingPlaceholder()))
{}

Card ConstrainedCard::get_card() const
{
  return boost::apply_visitor(get_card_visitor(), card_);
}

bool operator<(ConstrainedCard const& l, ConstrainedCard const& r)
{
  return boost::apply_visitor(compare_visitor_1(l), r.card_);
}

}

