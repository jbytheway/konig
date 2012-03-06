#include <konig/ai/feature.hpp>

#include <konig/utility/make_unique.hpp>

namespace konig { namespace ai {

namespace features {

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
      result += hand.subset(s).empty();
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

struct TotalTrumpRank : Feature {
  virtual std::string name() const { return "totaltrumprank"; }
  virtual double compute(Cards const& hand) const {
    int result = 0;
    BOOST_FOREACH(auto const& c, hand.equal_range(Suit::trumps)) {
      result += c.trump_rank();
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

struct Pagat : Feature {
  virtual std::string name() const { return "pagat"; }
  virtual double compute(Cards const& hand) const {
    return hand.count(TrumpRank::pagat);
  }
};

} // end namespace features

std::vector<Feature::Ptr> Feature::default_feature_sequence()
{
  using konig::utility::make_unique;
  std::vector<Feature::Ptr> result;
  result.push_back(std::make_shared<features::NumTrumps>());
  result.push_back(std::make_shared<features::NumKings>());
  result.push_back(std::make_shared<features::NumVoids>());
  result.push_back(std::make_shared<features::NumTopTrumps>());
  result.push_back(std::make_shared<features::TotalTrumpRank>());
  result.push_back(std::make_shared<features::CardPoints>());
  result.push_back(std::make_shared<features::Pagat>());
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

