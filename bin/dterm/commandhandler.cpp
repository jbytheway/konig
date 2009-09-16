#include "commandhandler.hpp"

namespace konig { namespace dterm {

CommandHandler::CommandHandler(konig::client::ServerInterface& si) :
  server_interface_(si),
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

void CommandHandler::command(std::string const& c)
{
  if (c.empty()) return;
  assert(output_);
  output_->message("you gave the command '"+c+"'");
}

void CommandHandler::end()
{
  assert(output_);
  output_->message("Exit");
  output_->interrupt();
}

}}

