#ifndef KONIG_DTERM__COMMANDHANDLER_HPP
#define KONIG_DTERM__COMMANDHANDLER_HPP

#include <konig/client/serverinterface.hpp>

#include "messagesink.hpp"
#include "gametracker.hpp"

namespace konig { namespace dterm {

class CommandHandler : public client::ClientInterface {
  public:
    CommandHandler();
    void set_output(MessageSink&);
    void unset_output();
    void set_server_interface(konig::client::ServerInterface&);
    void unset_server_interface();

    // Functions used by UI side
    void command(std::string const&);
    void end();

    // Functions used by ServerInterface side
    virtual void warning(const std::string&);
    virtual void abort();
    virtual Player& player();
  private:
    konig::client::ServerInterface* server_interface_;
    MessageSink* output_;
    GameTracker tracker_;
};

}}

#endif // KONIG_DTERM__COMMAND_HANDLER_HPP

