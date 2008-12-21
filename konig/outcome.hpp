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
    Outcome(boost::shared_ptr<Contract>);

    void add(bool offence, Feat, Announcedness, Achievement);
  private:
    boost::shared_ptr<Contract> contract_;
    typedef std::map<
        std::pair<Feat, bool>,
        std::pair<Announcedness, Achievement>
      > Results;
    Results results_;
};

}

#endif // KONIG__OUTCOME_HPP

