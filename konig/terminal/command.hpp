#ifndef KONIG_TERMINAL_COMMAND_HPP
#define KONIG_TERMINAL_COMMAND_HPP

#include <list>
#include <set>

#include <boost/function.hpp>

namespace konig { namespace terminal {

class CommandParser;

class Command {
  public:
    typedef std::set<std::string> Names;
    typedef boost::function<void (CommandParser&, std::list<std::string>)>
      Implementation;
    Command(
        Names names,
        Implementation implementation,
        std::string short_usage,
        std::string long_usage
      );

    Names const& names() const { return names_; }
    Implementation const& implementation() const { return implementation_; }
    std::string const& short_usage() const { return short_usage_; }
    std::string const& long_usage() const { return long_usage_; }
  private:
    Names names_;
    Implementation implementation_;
    std::string short_usage_;
    std::string long_usage_;
};

}}

#endif // KONIG_TERMINAL_COMMAND_HPP

