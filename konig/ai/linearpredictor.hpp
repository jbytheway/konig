#ifndef KONIG_AI_LINEARPREDICTOR_HPP
#define KONIG_AI_LINEARPREDICTOR_HPP

#include <konig/ai/feature.hpp>

namespace konig { namespace ai {

class LinearPredictor {
  public:
    LinearPredictor(double intercept);

    void add(Feature::Ptr, double coefficient);
    double predict(Cards const& hand) const;
  private:
    double intercept_;
    std::vector<std::pair<Feature::Ptr, double>> coefficients_;
};

}}

#endif // KONIG_AI_LINEARPREDICTOR_HPP

