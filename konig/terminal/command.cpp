#include <konig/terminal/command.hpp>

namespace konig { namespace terminal {

Command::Command(
  std::set<std::string> n,
  Implementation impl,
  std::string shortUsage,
  std::string longUsage
) :
  names_(std::move(n)),
  implementation_(impl),
  short_usage_(std::move(shortUsage)),
  long_usage_(std::move(longUsage))
{}

}}

