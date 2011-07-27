#include <konig/terminal/relasiomessagesink.hpp>

namespace konig { namespace terminal {

RelasioMessageSink::RelasioMessageSink(relasio::readline& rl) :
  rl_(rl)
{}

void RelasioMessageSink::message(std::string const& message)
{
  rl_.write(message);
}

void RelasioMessageSink::set_prompt(std::string const& prompt)
{
  rl_.set_prompt(prompt);
}

void RelasioMessageSink::interrupt()
{
  rl_.stop();
}

}}

