#ifndef KONIG__FIELDS_HPP
#define KONIG__FIELDS_HPP

#include <boost/preprocessor/iteration/local.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/core.hpp>

namespace konig { namespace fields {

class id;
class name;
class reason;
class value;
class rules;
class position;
class cards;
class bid;
class king;
class talon;
class choice;
class discard;
class announcements;
class card;
class message;

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

// This serialization function should be found by ADL when the fusion::map keys
// are konig::fields.
// TODO: Switch to nice variadic implementation once it makes it into boost.
#define BOOST_PP_LOCAL_MACRO(n)                                       \
template<class Archive, BOOST_PP_ENUM_PARAMS(n, typename T)>          \
void serialize(                                                       \
    Archive& archive,                                                 \
    fusion::map<BOOST_PP_ENUM_PARAMS(n, T)>& m, const unsigned int) { \
  fusion::for_each(m, Serializer<Archive>(archive));                  \
}

#define BOOST_PP_LOCAL_LIMITS (1, FUSION_MAX_MAP_SIZE)
#include BOOST_PP_LOCAL_ITERATE()

}}

#endif // KONIG__FIELDS_HPP

