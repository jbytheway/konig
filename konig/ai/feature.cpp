#include <konig/ai/feature.hpp>

#include <konig/utility/make_unique.hpp>

namespace konig { namespace ai {

struct NumTrumps : Feature {
  virtual std::string name() const { return "numtrumps"; }
  virtual double compute(Cards const& hand) const {
    return hand.count(Suit::trumps);
  }
};

struct NumKings : Feature {
  virtual std::string name() const { return "numkings"; }
  virtual double compute(Cards const& hand) const {
    return hand.count(SuitRank::king);
  }
};

struct NumVoids : Feature {
  virtual std::string name() const { return "numvoids"; }
  virtual double compute(Cards const& hand) const {
    int result = 0;
    for (Suit s = Suit::min; s < Suit::trumps; ++s) {
      result += 0 == hand.count(s);
    }
    return result;
  }
};

struct NumTopTrumps : Feature {
  virtual std::string name() const { return "numtoptrumps"; }
  virtual double compute(Cards const& hand) const {
    int result = 0;
    TrumpRank r = TrumpRank::skus;
    while (hand.count(Card(r))) {
      --r;
      assert(r >= TrumpRank::pagat);
      ++result;
    }
    return result;
  }
};

struct CardPoints : Feature {
  virtual std::string name() const { return "cardpoints"; }
  virtual double compute(Cards const& hand) const {
    return hand.total_card_points();
  }
};

std::vector<Feature::Ptr> Feature::default_feature_sequence()
{
  using konig::utility::make_unique;
  std::vector<Feature::Ptr> result;
  result.push_back(std::make_shared<NumTrumps>());
  result.push_back(std::make_shared<NumKings>());
  result.push_back(std::make_shared<NumVoids>());
  result.push_back(std::make_shared<NumTopTrumps>());
  result.push_back(std::make_shared<CardPoints>());
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

