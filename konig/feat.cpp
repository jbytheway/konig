#include <konig/feat.hpp>

#include <konig/contract.hpp>

namespace konig {

Achievement Feat::result_for(
    const boost::shared_ptr<Contract>& contract,
    Card called_king,
    const std::vector<Trick>& tricks,
    const Cards& declarers_cards,
    const Cards& defences_cards,
    bool feat_offensive,
    bool offence[4]
  ) const
{
  const Cards& our_cards = feat_offensive ? declarers_cards : defences_cards;
  const Cards& their_cards = feat_offensive ? defences_cards : declarers_cards;

  switch (value_) {
    case game:
      assert(feat_offensive);
      return contract->result_for(declarers_cards) ?
        Achievement::made : Achievement::off;
    case forty_five:
      // Remember to cater for 45 via valat
      return
        ( our_cards.total_card_points() >= 45 ||
          their_cards.size() == 6) ?
        Achievement::made : Achievement::neutral;
    case valat:
      return
        ( their_cards.size() == 0 ||
          their_cards.size() == 3 ||
          their_cards.size() == 6) ?
        Achievement::made : Achievement::neutral;
    case king_ultimo:
      {
        const Trick& last_trick = tricks.back();
        if (last_trick.count(called_king)) {
          bool winning_side = offence[last_trick.winner()];
          return (winning_side == feat_offensive) ?
            Achievement::made : Achievement::off;
        } else {
          return Achievement::neutral;
        }
      }
    case pagat:
    case uhu:
    case kakadu:
      {
        const Trick& trick = tricks[12-constrained_to_trick()];
        if (trick.count(constrained_card(called_king))) {
          bool winning_side = offence[trick.winner()];
          const Card winning_card = trick.winning_card();
          return
            ( winning_side == feat_offensive &&
              winning_card == constrained_card(called_king)) ?
            Achievement::made : Achievement::off;
        } else {
          return Achievement::neutral;
        }
      }
    default:
      throw std::logic_error("unknown feat");
  }
}

}

