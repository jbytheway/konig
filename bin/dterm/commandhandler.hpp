#ifndef KONIG_DTERM__COMMANDHANDLER_HPP
#define KONIG_DTERM__COMMANDHANDLER_HPP

#include <konig/client/serverinterface.hpp>

#include "messagesink.hpp"

namespace konig { namespace dterm {

class CommandHandler {
  public:
    CommandHandler(konig::client::ServerInterface&);
    void set_output(MessageSink&);
    void unset_output();
    void command(std::string const&);
    void end();
  private:
    konig::client::ServerInterface& server_interface_;
    MessageSink* output_;
};

}}

#endif // KONIG_DTERM__COMMAND_HANDLER_HPP

