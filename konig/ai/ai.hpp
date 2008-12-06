#ifndef KONIG_AI__AI_HPP
#define KONIG_AI__AI_HPP

#include <konig/player.hpp>

namespace konig { namespace ai {

class Ai : public Player {
  public:
    typedef boost::shared_ptr<Ai> Ptr;

    static Ptr create(const std::string& description);

    Ai();

    virtual void start_game(const Ruleset&, PlayPosition, const Cards& hand);
    virtual void notify_bid(int);
  private:
    Ruleset rules_;
    PlayPosition position_;
    Cards hand_;
    std::vector<int> bidding_;
};

}}

#endif // KONIG_AI__AI_HPP

