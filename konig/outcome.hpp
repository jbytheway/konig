#ifndef KONIG__OUTCOME_HPP
#define KONIG__OUTCOME_HPP

#include <map>

#include <konig/api.hpp>
#include <konig/achievement.hpp>
#include <konig/feat.hpp>
#include <konig/announcednesses.hpp>

namespace konig {

class Contract;

class KONIG_API Outcome {
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
    std::string string() const;

    void add(
      bool offence,
      Feat,
      Announcedness,
      Achievement,
      bool against_three,
      Announcednesses const&
    );

    std::array<int, 4>
    compute_scores(std::array<bool, 4> const& achievers) const;
  private:
    boost::shared_ptr<Contract const> contract_;
    uint8_t num_game_achievers_;
    int achiever_score_;
    Results results_;
};

KONIG_API std::ostream& operator<<(std::ostream&, const Outcome&);

}

#endif // KONIG__OUTCOME_HPP

