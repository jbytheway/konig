#include <konig/outcome.hpp>

#include <konig/contract.hpp>

namespace konig {

Outcome::Outcome(Contract::ConstPtr contract) :
  contract_(std::move(contract))
{}

void Outcome::add(bool offence, Feat f, Announcedness an, Achievement ac)
{
  if (an == Announcedness::unannounced && ac == Achievement::neutral)
    return;
  // If it was announced and didn't make, then it's off
  if (ac == Achievement::neutral)
    ac = Achievement::off;
  results_[std::make_pair(f, offence)] = std::make_pair(an, ac);
}

std::ostream& operator<<(std::ostream& o, const Outcome& outcome)
{
  o << outcome.contract()->short_name();
  for (Outcome::Results::const_iterator i = outcome.results().begin();
      i != outcome.results().end(); ++i) {
    Feat f = i->first.first;
    bool defensive = !i->first.second;
    Announcedness announcedness = i->second.first;
    Achievement achievement = i->second.second;
    if (announcedness != Announcedness::unannounced ||
        achievement != Achievement::neutral) {
      o << f;
      if (defensive) {
        o << '!';
      }

      // Fake the announcedness of the game to get the right output format
      if (f == Feat::game && announcedness == Announcedness::announced) {
        announcedness = Announcedness::unannounced;
      }
      o << announcedness.string(achievement);
    }
  }
  return o;
}

}

