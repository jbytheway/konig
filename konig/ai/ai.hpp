#ifndef KONIG_AI__AI_HPP
#define KONIG_AI__AI_HPP

#include <konig/player.hpp>

namespace konig { namespace ai {

class Ai : public Player {
  public:
    typedef boost::shared_ptr<Ai> Ptr;

    static Ptr create(const std::string& description);
};

}}

#endif // KONIG_AI__AI_HPP

