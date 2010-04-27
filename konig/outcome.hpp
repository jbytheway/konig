#ifndef KONIG__OUTCOME_HPP
#define KONIG__OUTCOME_HPP

#include <map>

#include <konig/achievement.hpp>
#include <konig/feat.hpp>
#include <konig/announcedness.hpp>

namespace konig {

class Contract;

class Outcome {
  public:
    typedef std::map<
        std::pair<Feat, bool>,
        std::pair<Announcedness, Achievement>
      > Results;

    Outcome() = default;
    Outcome(boost::shared_ptr<Contract const>, uint8_t num_game_achievers);

    const boost::shared_ptr<Contract const>& contract() const {
      return contract_;
    }
    uint8_t num_game_achievers() const { return num_game_achievers_; }
    const Results& results() const { return results_; }

    void add(bool offence, Feat, Announcedness, Achievement);

    std::array<int, 4>
    compute_scores(std::array<bool, 4> const& achievers) const;
  private:
    boost::shared_ptr<Contract const> contract_;
    uint8_t num_game_achievers_;
    int achiever_score_;
    Results results_;
};

std::ostream& operator<<(std::ostream&, const Outcome&);

}

#endif // KONIG__OUTCOME_HPP

