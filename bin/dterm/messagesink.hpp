#ifndef KONIG_DTERM__MESSAGESINK_HPP
#define KONIG_DTERM__MESSAGESINK_HPP

namespace konig { namespace dterm {

/* Abstract base class for sending messages to user and interrupting the UI */
class MessageSink {
  public:
    virtual void message(std::string const&) = 0;
    virtual void interrupt() = 0;
  protected:
    MessageSink() {}
    MessageSink(MessageSink const&) {}
    MessageSink& operator=(MessageSink const&) { return *this; }
    ~MessageSink() {}
};

}}

#endif // KONIG_DTERM__MESSAGESINK_HPP
