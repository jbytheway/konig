#include <konig/ai/feature.hpp>

#include <konig/utility/make_unique.hpp>

namespace konig { namespace ai {

struct NumTrumps : Feature {
  virtual std::string name() const { return "numtrumps"; }
  virtual double compute(Cards const& hand) const {
    return hand.count(Suit::trumps);
  }
};

std::vector<Feature::Ptr> Feature::default_feature_sequence()
{
  using konig::utility::make_unique;
  std::vector<Feature::Ptr> result;
  result.push_back(std::make_shared<NumTrumps>());
  return result;
}

std::map<std::string, Feature::Ptr> Feature::default_feature_map()
{
  std::map<std::string, Feature::Ptr> result;
  for (auto const& p : default_feature_sequence()) {
    result.insert({p->name(), p});
  }
  return result;
}

}}

