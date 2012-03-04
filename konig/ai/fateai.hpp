#ifndef KONIG_AI_FATEAI_HPP
#define KONIG_AI_FATEAI_HPP

#include <konig/utility/smallset.hpp>
#include <konig/ai/ai.hpp>
#include <konig/ai/cardfate.hpp>

namespace konig { namespace ai {

// Wrapper on top of Ai that also keeps track of the possible fates of each
// card in the deck
class KONIG_AI_API FateAi : public Ai {
  public:
    typedef utility::SmallSet<CardFate::internal_enum, CardFate::max> CardFates;
    typedef std::map<Card, CardFates> CardsFates;

    FateAi();

    virtual void start_game(Ruleset, PlayPosition, Cards hand);
    virtual void notify_call_king(KingCall);
    virtual void notify_talon(const std::array<Cards, 2>& talon);
    virtual void notify_talon_choice(uint8_t);
    virtual void notify_discard(Cards);
    virtual void notify_play_card(PlayPosition, Card);

    virtual void trick_complete_hook();

    Suit called_suit(int fallback = Suit::max) const;
    bool is_called_suit(Suit) const;
    bool guess_is_partner(PlayPosition) const;
    bool guess_is_on_my_side(PlayPosition) const;
    int seats_after_declarer() const;
    bool had_first_round(Suit) const;
    CardFates fates_of(Card const&) const;
    CardFates fates_of(TrumpRank) const;
    CardFates other_players_hands() const;
    std::pair<CardsFates::iterator, CardsFates::iterator> fates_of(Suit const);
    std::pair<CardsFates::const_iterator, CardsFates::const_iterator>
      fates_of(Suit) const;
    Cards trumps_in(CardFates const&) const;
    Cards trumps_out() const;
    bool trumps_known_exhausted() const;
    size_t num_players_known_out_of_trumps() const;
    bool
      guaranteed_to_win_against(Card const&, CardFates const&) const;
    bool guaranteed_to_win_against(Card const&, PlayPosition) const;
    size_t num_tricks_taken_by(PlayPosition) const;
    bool cards_are_equivalent(Cards const&) const;
  private:
    std::array<bool, Suit::max> had_first_round_;
    std::array<size_t, position_max> num_tricks_taken_by_;
    Suit called_suit_;
    CardsFates fates_;
};

}}

#endif // KONIG_AI_FATEAI_HPP

