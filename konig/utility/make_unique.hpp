#ifndef KONIG_UTILITY_MAKE_UNIQUE_HPP
#define KONIG_UTILITY_MAKE_UNIQUE_HPP

namespace konig { namespace utility {

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}}

#endif // KONIG_UTILITY_MAKE_UNIQUE_HPP

