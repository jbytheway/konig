#ifndef KONIG_AI_FEATURE_HPP
#define KONIG_AI_FEATURE_HPP

#include <vector>
#include <map>

#include <konig/cards.hpp>

namespace konig { namespace ai {

class Feature : boost::noncopyable {
  public:
    typedef std::shared_ptr<Feature const> Ptr;

    virtual std::string name() const = 0;
    virtual double compute(
      Cards const& hand,
      Cards const& discard,
      Cards const& rejected_half
    ) const = 0;

    static std::vector<Ptr> default_feature_sequence();
    static std::map<std::string, Ptr> default_feature_map();
  protected:
    Feature() = default;
};

}}

#endif // KONIG_AI_FEATURE_HPP

