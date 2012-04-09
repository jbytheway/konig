#include <konig/outcome.hpp>

#include <numeric>

#include <boost/lexical_cast.hpp>

#include <konig/contract.hpp>

namespace konig {

Outcome::Outcome(
  Contract const& contract,
  uint8_t num_game_achievers,
  bool conceded
) :
  contract_(contract.shared_from_this()),
  num_game_achievers_(num_game_achievers),
  conceded_(conceded),
  achiever_score_{0}
{}

std::string Outcome::string() const {
  // Maybe too slow...
  return boost::lexical_cast<std::string>(*this);
}

void Outcome::add(
  bool offence,
  Feat f,
  Announcedness an,
  Achievement ac,
  bool against_three,
  Announcednesses const& announcednesses
)
{
  if (an == Announcedness::unannounced && ac == Achievement::neutral)
    return;
  // If it was announced and didn't make, then it's off
  if (ac == Achievement::neutral)
    ac = Achievement::off;
  assert(!results_.count(std::make_pair(f, offence)));
  results_[std::make_pair(f, offence)] = std::make_pair(an, ac);
  int score = contract().value_of(f, an, ac, against_three, announcednesses);
  if (offence) {
    achiever_score_ += score;
  } else {
    achiever_score_ -= score;
  }
}

std::array<int, 4>
Outcome::compute_scores(std::array<bool, 4> const& achievers) const
{
  assert(
    num_game_achievers_ ==
    std::accumulate(achievers.begin(), achievers.end(), 0)
  );
  int achiever_score, other_score;
  switch (num_game_achievers_) {
    case 1:
      achiever_score = 3*achiever_score_;
      other_score = -achiever_score_;
      break;
    case 2:
      achiever_score = achiever_score_;
      other_score = -achiever_score_;
      break;
    // 3 and 4 can only happen in Trischaken
    case 3:
      achiever_score = achiever_score_;
      other_score = -3*achiever_score_;
      break;
    case 4:
      achiever_score = 0;
      other_score = 0;
      break;
    default:
      throw std::logic_error("invalid number of achievers");
  }
  std::array<int, 4> scores;
  for (size_t i=0; i<4; ++i) {
    scores[i] = ( achievers[i] ? achiever_score : other_score );
  }
  return scores;
}

bool operator<(Outcome const& l, Outcome const& r)
{
  return l.string() < r.string();
}

std::ostream& operator<<(std::ostream& o, const Outcome& outcome)
{
  for (Outcome::Results::const_iterator i = outcome.results().begin();
      i != outcome.results().end(); ++i) {
    Feat f = i->first.first;
    bool defensive = !i->first.second;
    Announcedness announcedness = i->second.first;
    Achievement achievement = i->second.second;
    if (f == Feat::game) {
      o << outcome.contract().outcome_name(
          outcome.num_game_achievers(), announcedness, achievement,
          outcome.conceded()
        );
    } else {
      assert(
          announcedness != Announcedness::unannounced ||
          achievement != Achievement::neutral
        );
      o << f;
      if (defensive) o << '!';
      o << announcedness.string(achievement);
    }
  }
  return o;
}

}

