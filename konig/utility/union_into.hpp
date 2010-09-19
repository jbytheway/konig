#ifndef KONIG_UTILITY__UNION_INTO_HPP
#define KONIG_UTILITY__UNION_INTO_HPP

namespace konig { namespace utility {

template<typename SimpleAssociativeContainer>
void union_into(
  SimpleAssociativeContainer& lhs,
  SimpleAssociativeContainer const& rhs
)
{
  SimpleAssociativeContainer result;
  std::set_union(
    lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
    std::inserter(result, result.begin())
  );
  lhs = std::move(result);
}

}}

#endif // KONIG_UTILITY__UNION_INTO_HPP

