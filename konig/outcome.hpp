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

    Outcome() {}
    Outcome(boost::shared_ptr<Contract const>);

    const boost::shared_ptr<Contract const>& contract() const {
      return contract_;
    }
    const Results& results() const { return results_; }

    void add(bool offence, Feat, Announcedness, Achievement);
  private:
    boost::shared_ptr<Contract const> contract_;
    Results results_;
};

std::ostream& operator<<(std::ostream&, const Outcome&);

}

#endif // KONIG__OUTCOME_HPP

