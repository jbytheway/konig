#ifndef KONIG_REAL_COMMANDHANDLER_HPP
#define KONIG_REAL_COMMANDHANDLER_HPP

#include <boost/noncopyable.hpp>

#include <konig/terminal/messagesink.hpp>

namespace konig { namespace real {

class CommandHandler : boost::noncopyable {
  public:
    CommandHandler();
    ~CommandHandler();
    void set_output(terminal::MessageSink&);
    void unset_output();

    // Called by command line
    void command(std::string const&);
    void end();
  private:
    terminal::MessageSink* output_;
};

}}

#endif // KONIG_REAL_COMMANDHANDLER_HPP

