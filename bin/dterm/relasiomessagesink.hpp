#ifndef KONIG_DTERM_RELASIOMESSAGESINK_HPP
#define KONIG_DTERM_RELASIOMESSAGESINK_HPP

#include <relasio/readline.hpp>

#include "messagesink.hpp"

namespace konig { namespace dterm {

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

#endif // KONIG_DTERM_RELASIOMESSAGESINK_HPP

