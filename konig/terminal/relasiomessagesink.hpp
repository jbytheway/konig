#ifndef KONIG_TERMINAL_RELASIOMESSAGESINK_HPP
#define KONIG_TERMINAL_RELASIOMESSAGESINK_HPP

#include <relasio/readline.hpp>

#include <konig/terminal/messagesink.hpp>

namespace konig { namespace terminal {

class RelasioMessageSink : public MessageSink {
  public:
    RelasioMessageSink(relasio::readline&);

    virtual void message(std::string const&);
    virtual void set_prompt(std::string const&);
    virtual void interrupt();
  private:
    relasio::readline& rl_;
};

}}

#endif // KONIG_TERMINAL_RELASIOMESSAGESINK_HPP

