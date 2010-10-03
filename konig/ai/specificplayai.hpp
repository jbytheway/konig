#ifndef KONIG_AI__SPECIFICPLAYAI_HPP
#define KONIG_AI__SPECIFICPLAYAI_HPP

#include <list>
#include <random>

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

class KONIG_AI_API SpecificPlayAi : public PlayAi {
  public:
    SpecificPlayAi(const std::string& play_sequence = "");
    SpecificPlayAi(const std::vector<Card>& play_sequence);

    virtual void reset(FateAi const&);
    virtual std::vector<Announcement> announce(FateAi const&);
    virtual Card play_card(FateAi const&);

    struct PlayRule {
      typedef boost::shared_ptr<PlayRule> Ptr;

      virtual bool apply(const Trick&, const Cards& legal, Card& play) = 0;
    };
  private:
    void init_play_rules(const std::vector<Card>& play_sequence);
    void init_play_rules(const std::string& play_sequence);

    std::mt19937 random_engine_;
    std::vector<std::list<PlayRule::Ptr> > play_rules_;
};

}}

#endif // KONIG_AI__SPECIFICPLAYAI_HPP

