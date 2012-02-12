#ifndef KONIG_PLAYERS_HPP
#define KONIG_PLAYERS_HPP

#include <boost/iterator/indirect_iterator.hpp>

#include <konig/player.hpp>

namespace konig {

class Players {
  public:
    template<typename Range>
    Players(Range const& ps) :
      players_(std::begin(ps), std::end(ps))
    {
      assert(players_.size() == 4);
    }

    typedef std::vector<boost::shared_ptr<Player>> Storage;

    typedef boost::indirect_iterator<Storage::const_iterator> const_iterator;

    Player& operator[](size_t const i) const {
      assert(i < 4);
      return *players_[i];
    }

    const_iterator begin() const { return players_.begin(); }
    const_iterator end() const { return players_.end(); }
  private:
    Storage players_;
};

}

#endif // KONIG_PLAYERS_HPP

