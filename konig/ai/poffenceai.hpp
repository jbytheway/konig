#ifndef KONIG_AI_POFFENCEAI_HPP
#define KONIG_AI_POFFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class POffenceAi : public PlayAi {
  public:
    virtual void reset(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
    template<typename Container, typename F>
    void erase_bad_candidates(Container& candidates, F const& f);

    Cards target_cards_;
};

}}

#endif // KONIG_AI_POFFENCEAI_HPP

