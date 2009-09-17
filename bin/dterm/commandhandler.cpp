#include "commandhandler.hpp"

namespace konig { namespace dterm {

CommandHandler::CommandHandler() :
  server_interface_(NULL),
  output_(NULL)
{}

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
  output_->message("you gave the command '"+c+"'");
}

void CommandHandler::end()
{
  assert(server_interface_);
  assert(output_);
  server_interface_->close();
  output_->message("Exit");
  output_->interrupt();
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

}}

