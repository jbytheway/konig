#ifndef KONIG_DTERM_COMMANDHANDLER_HPP
#define KONIG_DTERM_COMMANDHANDLER_HPP

#include <konig/client/serverinterface.hpp>
#include <konig/terminal/commandhandler.hpp>

#include "gametracker.hpp"

namespace konig { namespace dterm {

class CommandHandler :
  public client::ClientInterface,
  public terminal::CommandHandler
{
  public:
    CommandHandler(boost::asio::io_service&);
    ~CommandHandler();
    void set_server_interface(konig::client::ServerInterface&);
    void unset_server_interface();

    // Functions used by UI side
    virtual void end();

    // Functions used by ServerInterface side
    virtual void message(std::string const&);
    virtual void warning(std::string const&);
    virtual void abort();
    virtual Player& player();

    // Functions used by GameTracker
    void present_bidding() const;
    void present_contract() const;
    void present_talon() const;
    void present_hand(bool with_accepted=false) const;
    void present_current_trick() const;

    // Functions to implement commands
    konig::client::ServerInterface& server_interface() {
      assert(server_interface_);
      return *server_interface_;
    }
  private:
    konig::client::ServerInterface* server_interface_;
    GameTracker tracker_;
};

}}

#endif // KONIG_DTERM_COMMAND_HANDLER_HPP

