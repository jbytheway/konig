#ifndef KONIG_AI__FATEAI_HPP
#define KONIG_AI__FATEAI_HPP

#include <konig/ai/ai.hpp>
#include <konig/ai/cardfate.hpp>

namespace konig { namespace ai {

// Wrapper on top of Ai that also keeps track of the possible fates of each
// card in the deck
class FateAi : public Ai {
  public:
    typedef std::map<Card, std::set<CardFate>> Fates;

    virtual void start_game(Ruleset, PlayPosition, Cards hand);
    virtual void notify_talon(const std::array<Cards, 2>& talon);
    virtual void notify_talon_choice(uint8_t);
    virtual void notify_discard(Cards);
    virtual void notify_play_card(PlayPosition, Card);

    std::pair<Fates::iterator, Fates::iterator> fates_of(Suit const);
    std::pair<Fates::const_iterator, Fates::const_iterator>
      fates_of(Suit const) const;
    Cards trumps_out() const;
    bool trumps_known_exhausted() const;
    bool guaranteed_to_win_against(Card const&, PlayPosition const) const;
  private:
    Fates fates_;
};

}}

#endif // KONIG_AI__FATEAI_HPP

