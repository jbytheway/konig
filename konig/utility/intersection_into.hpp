#ifndef KONIG_UTILITY__INTERSECTION_INTO_HPP
#define KONIG_UTILITY__INTERSECTION_INTO_HPP

namespace konig { namespace utility {

template<typename SimpleAssociativeContainer>
void intersection_into(
  SimpleAssociativeContainer& lhs,
  SimpleAssociativeContainer const& rhs
)
{
  SimpleAssociativeContainer result;
  std::set_intersection(
    lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
    std::inserter(result, result.begin())
  );
  lhs = std::move(result);
}

}}

#endif // KONIG_UTILITY__INTERSECTION_INTO_HPP

