#ifndef KONIG_AI__SPECIFICPLAYAI_HPP
#define KONIG_AI__SPECIFICPLAYAI_HPP

#include <list>
#include <random>

#include <konig/ai/ai.hpp>

namespace konig { namespace ai {

class SpecificPlayAi : public Ai {
  public:
    SpecificPlayAi(const std::string& play_sequence);

    virtual int bid();
    virtual KingCall call_king();
    virtual uint8_t choose_talon_half();
    virtual Cards discard();
    virtual std::vector<Announcement> announce();
    virtual Card play_card();

    struct PlayRule {
      typedef boost::shared_ptr<PlayRule> Ptr;

      virtual bool apply(const Trick&, const Cards& legal, Card& play) = 0;
    };
  private:
    std::mt19937 random_engine_;
    std::string contract_name_;
    std::vector<std::list<PlayRule::Ptr> > play_rules_;
};

}}

#endif // KONIG_AI__SPECIFICPLAYAI_HPP

