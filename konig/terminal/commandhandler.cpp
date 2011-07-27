#include "commandhandler.hpp"

#include <konig/fatal.hpp>

namespace konig { namespace terminal {

CommandHandler::CommandHandler() :
  output_(NULL)
{}

CommandHandler::~CommandHandler() = default;

void CommandHandler::set_output(terminal::MessageSink& output)
{
  output_ = &output;
}

void CommandHandler::unset_output()
{
  output_ = NULL;
}

void CommandHandler::command(std::string const&)
{
  KONIG_FATAL("not implemented");
}

void CommandHandler::end()
{
  assert(output_);
  output_->message("Exit");
  output_->interrupt();
}

}}

