#ifndef KONIG_UTILITY_INTERSECTS_HPP
#define KONIG_UTILITY_INTERSECTS_HPP

#include <set>

namespace konig { namespace utility {

template<typename T>
bool intersects(std::set<T> const& l, std::set<T> const& r)
{
  typedef typename std::set<T>::const_iterator it;
  it li = l.begin();
  it ri = r.begin();

  while (true) {
    if (li == l.end() || ri == r.end()) return false;
    if (*li == *ri) return true;
    if (*li < *ri) ++li; else ++ri;
  }
}

}}

#endif // KONIG_UTILITY_INTERSECTS_HPP

