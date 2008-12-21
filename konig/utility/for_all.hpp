#ifndef KONIG__UTILITY__FOR_ALL_HPP
#define KONIG__UTILITY__FOR_ALL_HPP

#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <konig/core.hpp>

namespace konig { namespace utility {

template<typename Range, typename Predicate>
inline bool for_all(const Range& r, const Predicate& p) {
  typedef typename boost::range_iterator<const Range>::type it;
  for (it i = boost::begin(r); i != boost::end(r); ++i) {
    if (!p(*i)) {
      return false;
    }
  }
  return true;
}

}}

#endif // KONIG__UTILITY__FOR_ALL_HPP

