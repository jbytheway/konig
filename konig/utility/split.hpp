#ifndef KONIG_UTILITY_SPLIT_HPP
#define KONIG_UTILITY_SPLIT_HPP

#include <boost/algorithm/string/split.hpp>

namespace konig { namespace utility {

std::vector<std::string> split(
  std::string const& s, char delimiter,
  boost::algorithm::token_compress_mode_type =
    boost::algorithm::token_compress_off
);

}}

#endif // KONIG_UTILITY_SPLIT_HPP

