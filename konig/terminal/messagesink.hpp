#ifndef KONIG_TERMINAL_MESSAGESINK_HPP
#define KONIG_TERMINAL_MESSAGESINK_HPP

#include <string>

namespace konig { namespace terminal {

/* Abstract base class for sending messages to user and interrupting the UI */
class MessageSink {
  public:
    virtual void message(std::string const&) = 0;
    virtual void set_prompt(std::string const&) = 0;
    virtual void interrupt() = 0;
  protected:
    MessageSink() {}
    MessageSink(MessageSink const&);
    MessageSink& operator=(MessageSink const&);
    ~MessageSink();
};

inline MessageSink::MessageSink(MessageSink const&) = default;
inline MessageSink& MessageSink::operator=(MessageSink const&) = default;
inline MessageSink::~MessageSink() = default;

}}

#endif // KONIG_TERMINAL_MESSAGESINK_HPP

