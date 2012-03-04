#ifndef KONIG_UTILITY_BITOPS_HPP
#define KONIG_UTILITY_BITOPS_HPP

#include <cassert>

namespace konig { namespace utility { namespace bitops {

template<typename T>
inline T ones(int count) {
  static_assert(std::is_unsigned<T>::value, "T must be an unsigned integer");
  constexpr int max_ones = std::numeric_limits<uintmax_t>::digits;
  assert(count >= 0 && count <= max_ones);
  if (count == max_ones) {
    return ~T(0);
  } else {
    return (T(1) << count)-1;
  }
}

#if defined(__GNUC__)

inline int trailing_zeros_unsafe(unsigned char x) {
  return __builtin_ctz(x);
}

inline int trailing_zeros_unsafe(unsigned short x) {
  return __builtin_ctz(x);
}

inline int trailing_zeros_unsafe(unsigned x) {
  return __builtin_ctz(x);
}

inline int trailing_zeros_unsafe(unsigned long x) {
  return __builtin_ctzl(x);
}

inline int trailing_zeros_unsafe(unsigned long long x) {
  return __builtin_ctzll(x);
}

inline int leading_zeros_unsafe(unsigned char x) {
  return __builtin_clz(x) - (
      std::numeric_limits<unsigned>::digits -
      std::numeric_limits<unsigned char>::digits);
}

inline int leading_zeros_unsafe(unsigned short x) {
  return __builtin_clz(x) - (
      std::numeric_limits<unsigned>::digits -
      std::numeric_limits<unsigned short>::digits);;
}

inline int leading_zeros_unsafe(unsigned x) {
  return __builtin_clz(x);
}

inline int leading_zeros_unsafe(unsigned long x) {
  return __builtin_clzl(x);
}

inline int leading_zeros_unsafe(unsigned long long x) {
  return __builtin_clzll(x);
}

template<typename T>
inline int trailing_zeros(T x) {
  static_assert(std::is_unsigned<T>::value, "T must be an unsigned integer");
  if (x) {
    return trailing_zeros_unsafe(x);
  } else {
    return std::numeric_limits<T>::digits;
  }
}

template<typename T>
inline int highest_bit(T x) {
  static_assert(std::is_unsigned<T>::value, "T must be an unsigned integer");
  return std::numeric_limits<T>::digits-1-leading_zeros_unsafe(x);
}

inline int popcount(unsigned char x) {
  return __builtin_popcount(x);
}

inline int popcount(unsigned short x) {
  return __builtin_popcount(x);
}

inline int popcount(unsigned x) {
  return __builtin_popcount(x);
}

inline int popcount(unsigned long x) {
  return __builtin_popcountl(x);
}

inline int popcount(unsigned long long x) {
  return __builtin_popcountll(x);
}

#else
#error Dont know how to do bitops on this compiler
#endif

}}}

#endif // KONIG_UTILITY_BITOPS_HPP

