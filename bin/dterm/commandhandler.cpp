#include "commandhandler.hpp"

#include <unordered_map>

#include <konig/fatal.hpp>

namespace konig { namespace dterm {

namespace {
  void get_setting(terminal::CommandParser& p, std::list<std::string> args) {
    if (args.size() == 1) {
      auto& name = args.front();
      static_cast<CommandHandler&>(p.handler()).server_interface().send(
        Message<MessageType::getSetting>(std::move(name))
      );
    } else {
      p.handler().output().message("Usage: get SETTING");
    }
  }

  void set_setting(terminal::CommandParser& p, std::list<std::string> args) {
    if (args.size() == 2) {
      std::string& name = args.front();
      std::string& value = args.back();
      static_cast<CommandHandler&>(p.handler()).server_interface().send(
        Message<MessageType::setSetting>(std::move(name), std::move(value))
      );
    } else {
      p.handler().output().message("Usage: set SETTING VALUE");
    }
  }

  void client_set_setting(
    terminal::CommandParser& p,
    std::list<std::string> args
  ) {
    if (args.size() == 2) {
      std::string name =
        "clients:" +
        static_cast<CommandHandler&>(p.handler()).server_interface().id().
        to_string() + ":" +
        args.front();
      std::string& value = args.back();
      static_cast<CommandHandler&>(p.handler()).server_interface().send(
        Message<MessageType::setSetting>(std::move(name), std::move(value))
      );
    } else {
      p.handler().output().message("Usage: set SETTING VALUE");
    }
  }

  void show_pid(terminal::CommandParser& p, std::list<std::string> args) {
    if (args.empty()) {
      std::string pid = boost::lexical_cast<std::string>(getpid());
      p.handler().output().message(pid);
    } else {
      p.handler().output().message("Usage: pid");
    }
  }
}

CommandHandler::CommandHandler(boost::asio::io_service& io) :
  terminal::CommandHandler(io, "> "),
  server_interface_(NULL),
  tracker_(*this)
{
  parser_->add_command(terminal::Command(
      { "g", "get" }, &get_setting,
      "Request the value of a setting",
      "get SETTING  Request the value of setting SETTING"
  ));
  parser_->add_command(terminal::Command(
      { "s", "set" }, &set_setting,
      "Request that server changes a setting",
      "set SETTING VALUE  Request that setting SETTING be set to VALUE"
  ));
  parser_->add_command(terminal::Command(
      { "cs", "clientset" }, &client_set_setting,
      "Request that server changes a setting within your client branch",
      "set SETTING VALUE  Request that setting clients:X:SETTING be set "
      "to VALUE, where X is your client id"
  ));
  parser_->add_command(terminal::Command(
      { "pid" }, &show_pid,
      "Print the process id of the client",
      "pid  Print the process id of the client, to facilitate attaching "
      "a debugger"
  ));
}

CommandHandler::~CommandHandler() = default;

void CommandHandler::set_server_interface(konig::client::ServerInterface& si)
{
  server_interface_ = &si;
}

void CommandHandler::unset_server_interface()
{
  server_interface_ = NULL;
}

void CommandHandler::end()
{
  assert(server_interface_);
  server_interface_->close();
  terminal::CommandHandler::end();
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
  output_->message("Network instigated abort");
  output_->interrupt();
}

Player& CommandHandler::player()
{
  return tracker_;
}

void CommandHandler::present_bidding() const
{
  Contracts const& contracts = tracker_.rules().contracts();
  std::string presentation = "The bidding so far is:\n";
  BOOST_FOREACH(Bid bid, tracker_.bidding()) {
    if (bid.is_pass()) {
      presentation += "pass ";
    } else {
      presentation += contracts.at(bid)->bid_name() + " ";
    }
  }
  output_->message(presentation);
}

void CommandHandler::present_contract() const
{
  std::string side;
  if (!tracker_.offence()) {
    side = "defending";
  } else if (tracker_.declarer() == tracker_.position()) {
    side = "declarer";
  } else {
    side = "partner";
  }
  output_->message(
      "The contract is " +
      tracker_.contract().string(tracker_.guess_num_offence()) +
      " and you are " + side
    );
}

void CommandHandler::present_talon() const
{
  std::ostringstream os;
  os << "The talon is:\n0: " <<
    tracker_.talon()[0] << "\n1: " << tracker_.talon()[1];
  output_->message(os.str());
}

void CommandHandler::present_hand(bool with_accepted) const
{
  Cards hand(tracker_.hand());
  if (with_accepted) {
    hand.insert(tracker_.accepted());
  }
  output_->message(
      "Your hand is:\n"+boost::lexical_cast<std::string>(hand)
    );
}

void CommandHandler::present_current_trick() const
{
  std::ostringstream os;
  Trick const& trick = tracker_.tricks().back();
  for (PlayPosition p = position_forehand; p < position_max; ++p) {
    int positionInTrick = (p+4-trick.leader())%4;
    if (positionInTrick < trick.played()) {
      os << trick.cards()[positionInTrick];
    } else {
      os << "--";
    }
    if (p == tracker_.declarer()) os << "#";
    if (p == tracker_.position()) os << "@";
    if (trick.played() && trick.winner() == p) os << "*";
    os << " ";
  }
  output_->message(os.str());
}

}}

