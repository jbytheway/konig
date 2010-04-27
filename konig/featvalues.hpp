#ifndef KONIG__FEATVALUES_HPP
#define KONIG__FEATVALUES_HPP

#include <map>

#include <boost/serialization/map.hpp>

#include <konig/feat.hpp>

namespace konig {

class FeatValues {
  public:
    FeatValues() = default; // For serialization

    int value_of(Feat, bool announced) const;

    static FeatValues cheltenham();

    FeatValues operator*(int);
  private:
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(value_map_);
    }

    typedef std::map<std::pair<Feat, bool>, int> ValueMap;

    FeatValues(ValueMap);

    ValueMap value_map_;
};

}

#endif // KONIG__FEATVALUES_HPP

