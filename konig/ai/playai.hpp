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
    virtual KingCall call_king(FateAi const&);
    virtual uint8_t choose_talon_half(FateAi const&);
    virtual Cards discard(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&) = 0;
    virtual void play_start(FateAi const&) {}
    virtual Card play_card(FateAi const&) = 0;
  protected:
    PlayAi() = default;
    PlayAi(PlayAi const&) = default;
    PlayAi& operator=(PlayAi const&) = default;
};

}}

#endif // KONIG_AI__PLAYAI_HPP

