#include "feature.hpp"

#include <konig/utility/make_unique.hpp>

namespace konig { namespace features {

struct NumTrumps : Feature {
  virtual std::string name() const { return "numtrumps"; }
  virtual double compute(Cards const& hand) const {
    return hand.count(Suit::trumps);
  }
};

std::vector<std::unique_ptr<Feature const>> Feature::default_features()
{
  using konig::utility::make_unique;
  std::vector<std::unique_ptr<Feature const>> result;
  result.push_back(make_unique<NumTrumps>());
  return result;
}

}}

