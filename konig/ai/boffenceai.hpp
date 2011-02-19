#ifndef KONIG_AI__BOFFENCEAI_HPP
#define KONIG_AI__BOFFENCEAI_HPP

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class BOffenceAi : public PlayAi {
  virtual void reset(FateAi const&);
  virtual Card play_card(FateAi const&);
};

}}

#endif // KONIG_AI__BOFFENCEAI_HPP

