#ifndef KONIG_TERMINAL_COMMANDPARSER_HPP
#define KONIG_TERMINAL_COMMANDPARSER_HPP

#include <unordered_map>

#include <boost/scoped_ptr.hpp>

#include <konig/terminal/command.hpp>
#include <konig/terminal/checker.hpp>

namespace konig { namespace terminal {

class CommandHandler;

class CommandParser {
  public:
    typedef CommandHandler handler_type;

    CommandParser(handler_type& handler);

    boost::scoped_ptr<Checker>& pre_checker() { return pre_checker_; }

    void command(std::list<std::string> tokens);
    void add_command(Command c);

    handler_type& handler() { return handler_; }
  private:
    void help(std::list<std::string> args);
    void quit(std::list<std::string> args);

    handler_type& handler_;
    typedef std::list<Command> Commands;
    Commands commands_;
    typedef std::unordered_map<std::string, Commands::iterator> CommandLookup;
    CommandLookup command_lookup_;
    boost::scoped_ptr<Checker> pre_checker_;
};

}}

#endif // KONIG_TERMINAL_COMMANDPARSER_HPP

