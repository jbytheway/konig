#ifndef KONIG_TERMINAL_COMMANDHANDLER_HPP
#define KONIG_TERMINAL_COMMANDHANDLER_HPP

#include <boost/noncopyable.hpp>

#include <konig/terminal/messagesink.hpp>

namespace konig { namespace terminal {

class CommandHandler : boost::noncopyable {
  public:
    CommandHandler();
    ~CommandHandler();
    void set_output(MessageSink&);
    void unset_output();

    // Called by command line
    virtual void command(std::string const&);
    virtual void end();
  protected:
    MessageSink* output_;
};

}}

#endif // KONIG_TERMINAL_COMMANDHANDLER_HPP

