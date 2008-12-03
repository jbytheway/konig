#ifndef KONIG__FIELDS_HPP
#define KONIG__FIELDS_HPP

#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/core.hpp>

namespace konig { namespace fields {

class name;

template<typename Archive>
class Serializer {
  public:
    Serializer(Archive& ar) : archive_(ar) {}
    template<typename Pair>
    void operator()(Pair& p) const {
      archive_ & boost::serialization::make_nvp("fusion_member", p.second);
    }
  private:
    Archive& archive_;
};

// This serialization function should be found by ADL
template<class Archive, class T0>
void serialize(Archive& archive, fusion::map<T0>& m, const unsigned int) {
  fusion::for_each(m, Serializer<Archive>(archive));
}

}}

#endif // KONIG__FIELDS_HPP

