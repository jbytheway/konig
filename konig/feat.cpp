#include <konig/feat.hpp>

#include <konig/contract.hpp>

namespace konig {

Achievement Feat::result_for(
    const boost::shared_ptr<Contract const>& contract,
    Card called_king,
    const std::vector<Trick>& tricks,
    const Cards& declarers_cards,
    const Cards& defences_cards,
    bool feat_offensive,
    std::array<bool, 4> const& achievers
  ) const
{
  const Cards& our_cards = feat_offensive ? declarers_cards : defences_cards;
  const Cards& their_cards = feat_offensive ? defences_cards : declarers_cards;

  switch (value_) {
    case game:
      assert(feat_offensive);
      return contract->result_for(declarers_cards, tricks);
    case forty_five:
      // Remember to cater for 45 via valat
      return
        ( our_cards.total_card_points() >= 45*3-1 ||
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
          bool winning_side = achievers[last_trick.winner()];
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
        PlayPosition played_by = trick.player_of(constrained_card(called_king));
        if (played_by != position_max &&
            achievers[played_by] == feat_offensive) {
          bool winning_side = achievers[trick.winner()];
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

