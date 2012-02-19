#include <konig/utility/split.hpp>

#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>

#include <konig/core.hpp>

namespace konig { namespace utility {

std::vector<std::string> split(
  std::string const& s, char delimiter,
  boost::algorithm::token_compress_mode_type eCompress
)
{
  std::vector<std::string> result;
  boost::algorithm::split(result, s, arg1 == delimiter, eCompress);
  return result;
}

}}

