#ifndef KONIG_DTERM__COMMANDHANDLER_HPP
#define KONIG_DTERM__COMMANDHANDLER_HPP

#include <boost/any.hpp>

#include <konig/fatal.hpp>
#include <konig/client/serverinterface.hpp>
#include <konig/client/asynccallerror.hpp>
#include <konig/terminal/messagesink.hpp>

#include "gametracker.hpp"
#include "uimode.hpp"

namespace konig { namespace dterm {

class CommandHandler : public client::ClientInterface {
  public:
    CommandHandler(boost::asio::io_service&);
    ~CommandHandler();
    void set_output(terminal::MessageSink&);
    void unset_output();
    void set_server_interface(konig::client::ServerInterface&);
    void unset_server_interface();

    // Functions used by UI side
    void command(std::string const&);
    void end();

    // Functions used by ServerInterface side
    virtual void message(std::string const&);
    virtual void warning(std::string const&);
    virtual void abort();
    virtual Player& player();

    // Functions used by GameTracker
    template<typename ReturnType>
    ReturnType get_from_user(UiMode const);
    void present_bidding() const;
    void present_contract() const;
    void present_talon() const;
    void present_hand(bool with_accepted=false) const;
    void present_current_trick() const;
  private:
    void set_mode(UiMode const);

    boost::asio::io_service& io_;
    konig::client::ServerInterface* server_interface_;
    terminal::MessageSink* output_;
    GameTracker tracker_;
    bool aborting_;
    UiMode mode_;

    class CommandParser;
    boost::scoped_ptr<CommandParser> parser_;

    boost::any return_value_;
};

template<typename ReturnType>
ReturnType CommandHandler::get_from_user(UiMode const mode)
{
  if (!return_value_.empty() || mode_ != UiMode::none) {
    KONIG_FATAL("re-entrant use of get_from_user");
  }
  set_mode(mode);
  do {
    io_.run_one();
    if (aborting_) {
      throw client::AsyncCallError();
    }
  } while (return_value_.empty());
  ReturnType returnValue = boost::any_cast<ReturnType>(return_value_);
  set_mode(UiMode::none);
  return_value_ = boost::any();
  return returnValue;
}

}}

#endif // KONIG_DTERM__COMMAND_HANDLER_HPP

