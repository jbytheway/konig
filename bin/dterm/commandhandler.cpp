#include "commandhandler.hpp"

#include <unordered_map>

#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <boost/spirit/home/phoenix/stl/container.hpp>
#include <boost/spirit/include/qi.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace dterm {

namespace {

// Classes inheriting from Checker provide a first-line check on commands to
// see if they are suitable for the currently required game mechanic (e.g. a
// bid)
class Checker {
  public:
    Checker(GameTracker const& t, boost::any& r) :
      tracker_(t),
      return_(r)
    {}
    virtual bool command(std::list<std::string> const& tokens) = 0;
  protected:
    GameTracker const& tracker_;
    boost::any& return_;
};

class BidChecker : public Checker {
  public:
    BidChecker(GameTracker const& t, boost::any& r) :
      Checker(t, r)
    {}

    virtual bool command(std::list<std::string> const& tokens) {
      if (tokens.size() != 1) return false;
      std::string const& bid = tokens.front();
      if (bid == "pass" || bid == "p") {
        return_ = -1;
        return true;
      }
      Contracts const& contracts = tracker_.rules().contracts();
      int index = contracts.index_by_short_name(bid);
      if (index != -1) {
        return_ = index;
        return true;
      }
      return false;
    }
};

}

class CommandHandler::CommandParser {
  public:
    typedef void (CommandParser::*Implementation)(std::list<std::string>);
    struct Command {
      Command(
          std::set<std::string> n,
          Implementation impl,
          std::string shortUsage,
          std::string longUsage
        ) :
        names(std::move(n)),
        implementation(impl),
        short_usage(std::move(shortUsage)),
        long_usage(std::move(longUsage))
      {}
      std::set<std::string> names;
      Implementation implementation;
      std::string short_usage;
      std::string long_usage;
    };

    CommandParser(CommandHandler& handler) :
      handler_(handler)
    {
      using boost::assign::list_of;
      add_command(Command(
            list_of("h")("help"), &CommandParser::help, "Get help",
            "help [COMMAND]  Display general help, or specific help on COMMAND"
          ));
      add_command(Command(
            list_of("q")("quit"), &CommandParser::quit, "Quit client",
            "quit  Disconnect from server and close client"
          ));
      add_command(Command(
            list_of("g")("get"), &CommandParser::get_setting,
            "Request the value of a setting",
            "get SETTING  Request the value of setting SETTING"
          ));
      add_command(Command(
            list_of("s")("set"), &CommandParser::set_setting,
            "Request that server changes a setting",
            "set SETTING VALUE  Request that setting SETTING be set to VALUE"
          ));
      add_command(Command(
            list_of("cs")("clientset"), &CommandParser::client_set_setting,
            "Request that server changes a setting within your client branch",
            "set SETTING VALUE  Request that setting clients:X:SETTING be set "
            "to VALUE, where X is your client id"
          ));
    }

    boost::scoped_ptr<Checker>& pre_checker() { return pre_checker_; }

    void command(std::list<std::string> tokens) {
      if (tokens.empty()) return;
      if (pre_checker_ && pre_checker_->command(tokens)) return;

      std::string name = std::move(tokens.front());
      tokens.pop_front();
      CommandLookup::iterator i = command_lookup_.find(name);
      if (i == command_lookup_.end()) {
        handler_.output_->message("No such command '"+name+"'");
      } else {
        (this->*i->second->implementation)(std::move(tokens));
      }
    }
  private:
    void add_command(Command c) {
      commands_.push_back(std::move(c));
      Commands::iterator newCommand = boost::prior(commands_.end());
      BOOST_FOREACH(std::string const& name, newCommand->names) {
        command_lookup_[name] = newCommand;
      }
    }

    void help(std::list<std::string> args) {
      if (args.empty()) {
        std::string message =
          "The following commands are available.  Do 'help COMMAND' for more "
          "details";
        size_t largest_width = 0;
        BOOST_FOREACH(Command const& c, commands_) {
          size_t width = 0;
          BOOST_FOREACH(std::string const& name, c.names) {
            width += name.size() + 2;
          }
          largest_width = std::max(largest_width, width);
        }
        BOOST_FOREACH(Command const& c, commands_) {
          size_t width = 0;
          message += "\n  ";
          bool started = false;
          BOOST_FOREACH(std::string const& name, c.names) {
            width += name.size() + 2;
            if (started) message += ", "; else started = true;
            message += name;
          }
          message += std::string(largest_width-width+2, ' ');
          message += c.short_usage;
        }
        handler_.output_->message(message);
      } else if (args.size() == 1) {
        std::string name = std::move(args.front());
        CommandLookup::iterator i = command_lookup_.find(name);
        if (i == command_lookup_.end()) {
          handler_.output_->message("No such command '"+name+"'");
        } else {
          handler_.output_->message(i->second->long_usage);
        }
      } else {
        handler_.output_->message("Usage: help [COMMAND]");
      }
    }

    void quit(std::list<std::string> args) {
      if (args.empty()) {
        handler_.end();
      } else {
        handler_.output_->message("Usage: quit");
      }
    }

    void get_setting(std::list<std::string> args) {
      if (args.size() == 1) {
        auto& name = args.front();
        handler_.server_interface_->send(Message<MessageType::getSetting>(
              std::move(name)
            ));
      } else {
        handler_.output_->message("Usage: get SETTING");
      }
    }

    void set_setting(std::list<std::string> args) {
      if (args.size() == 2) {
        std::string& name = args.front();
        std::string& value = args.back();
        handler_.server_interface_->send(Message<MessageType::setSetting>(
              std::move(name), std::move(value)
            ));
      } else {
        handler_.output_->message("Usage: set SETTING VALUE");
      }
    }

    void client_set_setting(std::list<std::string> args) {
      if (args.size() == 2) {
        std::string name =
          "clients:" + handler_.server_interface_->id().to_string() + ":" +
          args.front();
        std::string& value = args.back();
        handler_.server_interface_->send(Message<MessageType::setSetting>(
              std::move(name), std::move(value)
            ));
      } else {
        handler_.output_->message("Usage: set SETTING VALUE");
      }
    }

    CommandHandler& handler_;
    typedef std::list<Command> Commands;
    Commands commands_;
    typedef std::unordered_map<std::string, Commands::iterator> CommandLookup;
    CommandLookup command_lookup_;
    boost::scoped_ptr<Checker> pre_checker_;
};

CommandHandler::CommandHandler(boost::asio::io_service& io) :
  io_(io),
  server_interface_(NULL),
  output_(NULL),
  tracker_(*this),
  aborting_(false),
  mode_(UiMode::none),
  parser_(new CommandParser(*this)),
  expected_return_type_(NULL)
{}

CommandHandler::~CommandHandler() = default;

void CommandHandler::set_output(MessageSink& o)
{
  output_ = &o;
}

void CommandHandler::unset_output()
{
  output_ = NULL;
}

void CommandHandler::set_server_interface(konig::client::ServerInterface& si)
{
  server_interface_ = &si;
}

void CommandHandler::unset_server_interface()
{
  server_interface_ = NULL;
}

void CommandHandler::command(std::string const& c)
{
  if (c.empty()) return;
  assert(output_);
#if 0
  std::vector<std::string> tokens;
  // Requires Spirit 2.1, which isn't in Gentoo yet (it's also not finished)
  using namespace boost::spirit;
  using namespace boost::spirit::qi;
  typedef std::string::const_iterator it;
  qi::rule<it, std::string(), qi::ascii::space_type> unquoted =
    (~char_("\"'"))[_val += qi::_1] >> *(~char_(" \t\n\r\v\f"))[_val += qi::_1];
  it first = c.begin();
  bool r = phrase_parse(
      first, c.end(),
      *(
        unquoted
      ),
      qi::ascii::space,
      tokens
    );
  if (!r) {
    output_->message("error parsing command");
    return;
  }
  std::list<std::string> tokenList;
  std::move(tokens.begin(), tokens.end(), std::back_inserter(tokenList));
#else
  // Stop-gap parser until above usable
  std::list<std::string> tokenList;
  boost::algorithm::split(
      tokenList, c, boost::implicit_cast<int (*)(int)>(&std::isspace),
      boost::algorithm::token_compress_on
    );
  while (!tokenList.empty() && tokenList.front().empty()) {
    tokenList.pop_front();
  }
  while (!tokenList.empty() && tokenList.back().empty()) {
    tokenList.pop_back();
  }
  BOOST_FOREACH(std::string& token, tokenList) {
    if (token.empty()) {
      KONIG_FATAL("unexpected empty token");
    }
    if (token.size() == 1) continue;
    if (token[0] == '"' && *boost::prior(token.end()) == '"') {
      token.erase(token.begin());
      token.erase(boost::prior(token.end()));
    }
  }
#endif
  while (!tokenList.empty()) {
    auto semicolon = std::find(tokenList.begin(), tokenList.end(), ";");
    std::list<std::string> subcommand;
    subcommand.splice(
        subcommand.begin(), tokenList, tokenList.begin(), semicolon
      );
    parser_->command(std::move(subcommand));
    // Strip the semicolon
    if (!tokenList.empty()) tokenList.pop_front();
  }
}

void CommandHandler::end()
{
  assert(server_interface_);
  assert(output_);
  aborting_ = true;
  server_interface_->close();
  output_->message("Exit");
  output_->interrupt();
}

void CommandHandler::message(std::string const& s)
{
  assert(output_);
  output_->message(s);
}

void CommandHandler::warning(std::string const& s)
{
  assert(output_);
  output_->message(s);
}

void CommandHandler::abort()
{
  // Cannot assume server_interface_ is set in this function, because it may be
  // called from its constructor
  assert(output_);
  aborting_ = true;
  output_->message("Network instigated abort");
  output_->interrupt();
}

Player& CommandHandler::player()
{
  return tracker_;
}

void CommandHandler::set_mode(UiMode const mode)
{
  BOOST_STATIC_ASSERT(int(UiMode::max) == 2);
  switch (mode) {
    case UiMode::none:
      parser_->pre_checker().reset();
      output_->set_prompt("> ");
      break;
    case UiMode::bid:
      parser_->pre_checker().reset(new BidChecker(tracker_, return_value_));
      output_->set_prompt("bid> ");
      break;
    default:
      KONIG_FATAL("unexpected UiMode");
  }
}

}}

