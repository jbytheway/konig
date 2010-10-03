#ifndef KONIG_AI__PLAYAI_HPP
#define KONIG_AI__PLAYAI_HPP

#include <konig/ai/fateai.hpp>

namespace konig { namespace ai {

class PlayAi {
  public:
    typedef boost::shared_ptr<PlayAi> Ptr;

    static Ptr create(std::string const& description);

    virtual ~PlayAi() = 0;

    virtual void reset(FateAi const&) = 0;
    virtual Card play_card(FateAi const&) = 0;
  protected:
    PlayAi() = default;
    PlayAi(PlayAi const&) = default;
    PlayAi& operator=(PlayAi const&) = default;
};

}}

#endif // KONIG_AI__PLAYAI_HPP

