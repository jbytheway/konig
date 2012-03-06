#ifndef KONIG_UTILITY_UNION_INTO_HPP
#define KONIG_UTILITY_UNION_INTO_HPP

namespace konig { namespace utility {

template<typename T>
void union_into(std::set<T>& lhs, std::set<T> const& rhs)
{
  std::set<T> result;
  std::set_union(
    lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
    std::inserter(result, result.begin())
  );
  lhs = std::move(result);
}

}}

#endif // KONIG_UTILITY_UNION_INTO_HPP

