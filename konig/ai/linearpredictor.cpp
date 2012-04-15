#include <konig/ai/linearpredictor.hpp>

namespace konig { namespace ai {

LinearPredictor::LinearPredictor(double intercept) :
  intercept_(intercept)
{}

void LinearPredictor::add(Feature::Ptr feature, double coefficient)
{
  coefficients_.push_back({feature, coefficient});
}

double LinearPredictor::predict(Cards const& hand) const
{
  double tally = intercept_;
  for (auto const& p : coefficients_) {
    auto const& feature = p.first;
    double const coefficient = p.second;
    tally += coefficient * feature->compute(hand, Cards(), Cards());
  }
  return tally;
}

}}

