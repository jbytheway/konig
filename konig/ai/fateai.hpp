#ifndef KONIG_AI__FATEAI_HPP
#define KONIG_AI__FATEAI_HPP

#include <konig/ai/ai.hpp>
#include <konig/ai/cardfate.hpp>

namespace konig { namespace ai {

// Wrapper on top of Ai that also keeps track of the possible fates of each
// card in the deck
class KONIG_AI_API FateAi : public Ai {
  public:
    typedef std::map<Card, std::set<CardFate>> Fates;

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
    std::set<CardFate> fates_of(Card const&) const;
    std::set<CardFate> fates_of(TrumpRank) const;
    std::set<CardFate> other_players_hands() const;
    std::pair<Fates::iterator, Fates::iterator> fates_of(Suit const);
    std::pair<Fates::const_iterator, Fates::const_iterator>
      fates_of(Suit) const;
    Cards trumps_in(std::set<CardFate> const&) const;
    Cards trumps_out() const;
    bool trumps_known_exhausted() const;
    size_t num_players_known_out_of_trumps() const;
    bool
      guaranteed_to_win_against(Card const&, std::set<CardFate> const&) const;
    bool guaranteed_to_win_against(Card const&, PlayPosition) const;
    size_t num_tricks_taken_by(PlayPosition) const;
    bool cards_are_equivalent(Cards const&) const;
  private:
    std::array<bool, Suit::max> had_first_round_;
    std::array<size_t, position_max> num_tricks_taken_by_;
    Suit called_suit_;
    Fates fates_;
};

}}

#endif // KONIG_AI__FATEAI_HPP

