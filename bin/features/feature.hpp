#ifndef KONIG_FEATURES_FEATURE_HPP
#define KONIG_FEATURES_FEATURE_HPP

#include <konig/cards.hpp>

namespace konig { namespace features {

class Feature : boost::noncopyable {
  public:
    virtual std::string name() const = 0;
    virtual double compute(Cards const&) const = 0;

    static std::vector<std::unique_ptr<Feature const>> default_features();
  protected:
    Feature() = default;
};

}}

#endif // KONIG_FEATURES_FEATURE_HPP

