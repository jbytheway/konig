#include <konig/terminal/commandparser.hpp>

#include <boost/foreach.hpp>
#include <boost/next_prior.hpp>

#include <konig/terminal/commandhandler.hpp>

namespace konig { namespace terminal {

CommandParser::CommandParser(handler_type& handler) :
  handler_(handler)
{
  add_command(Command(
      { "h", "help" }, &CommandParser::help, "Get help",
      "help [COMMAND]  Display general help, or specific help on COMMAND"
  ));
  add_command(Command(
      { "q", "quit" }, &CommandParser::quit, "Quit client",
      "quit  Disconnect from server and close client"
  ));
}

void CommandParser::command(std::list<std::string> tokens) {
  if (tokens.empty()) return;
  if (pre_checker_ && pre_checker_->command(tokens)) return;

  std::string name = std::move(tokens.front());
  tokens.pop_front();
  CommandLookup::iterator i = command_lookup_.find(name);
  if (i == command_lookup_.end()) {
    handler_.output().message("No such command '"+name+"'");
  } else {
    i->second->implementation()(*this, std::move(tokens));
  }
}

void CommandParser::add_command(Command c) {
  commands_.push_back(std::move(c));
  Commands::iterator newCommand = boost::prior(commands_.end());
  BOOST_FOREACH(std::string const& name, newCommand->names()) {
    command_lookup_[name] = newCommand;
  }
}

void CommandParser::help(std::list<std::string> args) {
  if (args.empty()) {
    std::string message =
      "The following commands are available.  Do 'help COMMAND' for more "
      "details";
    size_t largest_width = 0;
    BOOST_FOREACH(Command const& c, commands_) {
      size_t width = 0;
      BOOST_FOREACH(std::string const& name, c.names()) {
        width += name.size() + 2;
      }
      largest_width = std::max(largest_width, width);
    }
    BOOST_FOREACH(Command const& c, commands_) {
      size_t width = 0;
      message += "\n  ";
      bool started = false;
      BOOST_FOREACH(std::string const& name, c.names()) {
        width += name.size() + 2;
        if (started) message += ", "; else started = true;
        message += name;
      }
      message += std::string(largest_width-width+2, ' ');
      message += c.short_usage();
    }
    handler_.output().message(message);
  } else if (args.size() == 1) {
    std::string name = std::move(args.front());
    CommandLookup::iterator i = command_lookup_.find(name);
    if (i == command_lookup_.end()) {
      handler_.output().message("No such command '"+name+"'");
    } else {
      handler_.output().message(i->second->long_usage());
    }
  } else {
    handler_.output().message("Usage: help [COMMAND]");
  }
}

void CommandParser::quit(std::list<std::string> args) {
  if (args.empty()) {
    handler_.end();
  } else {
    handler_.output().message("Usage: quit");
  }
}

}}

