#ifndef KONIG_TERMINAL_COMMANDHANDLER_HPP
#define KONIG_TERMINAL_COMMANDHANDLER_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <konig/terminal/messagesink.hpp>
#include <konig/terminal/commandparser.hpp>

namespace konig { namespace terminal {

class CommandHandler : boost::noncopyable {
  public:
    CommandHandler();
    ~CommandHandler();
    void set_output(MessageSink&);
    void unset_output();

    // Called by command line
    void command(std::string const&);
    virtual void end();

    MessageSink& output() { assert(output_); return *output_; }
  protected:
    MessageSink* output_;
    boost::scoped_ptr<CommandParser> parser_;
};

}}

#endif // KONIG_TERMINAL_COMMANDHANDLER_HPP

