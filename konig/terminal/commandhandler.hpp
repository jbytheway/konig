#ifndef KONIG_TERMINAL_COMMANDHANDLER_HPP
#define KONIG_TERMINAL_COMMANDHANDLER_HPP

#include <boost/any.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/asio/io_service.hpp>

#include <konig/fatal.hpp>
#include <konig/playerunavailableerror.hpp>
#include <konig/terminal/messagesink.hpp>
#include <konig/terminal/commandparser.hpp>
#include <konig/terminal/uimode.hpp>

namespace konig { namespace terminal {

class CommandHandler : boost::noncopyable {
  public:
    CommandHandler(boost::asio::io_service&, std::string prompt);
    ~CommandHandler();
    void set_output(MessageSink&);
    void unset_output();

    // Called by command line
    void command(std::string const&);
    virtual void end();

    // To pull a command from the user
    template<typename TUiMode, typename... Args>
    typename TUiMode::pre_checker_type::return_type
    get_from_user(Args&&...);

    MessageSink& output() { assert(output_); return *output_; }
  protected:
    boost::asio::io_service& io_;
    MessageSink* output_;

    std::string prompt_;
    boost::scoped_ptr<CommandParser> parser_;
    boost::scoped_ptr<UiMode> mode_;
    boost::any return_value_;
    bool aborting_;
};

template<typename TUiMode, typename... Args>
typename TUiMode::pre_checker_type::return_type
CommandHandler::get_from_user(Args&&... args)
{
  typedef typename TUiMode::pre_checker_type pre_checker_type;
  typedef typename pre_checker_type::return_type return_type;

  if (!return_value_.empty() || mode_) {
    KONIG_FATAL("re-entrant use of get_from_user");
  }
  mode_.reset(new TUiMode());
  parser_->pre_checker().reset(new pre_checker_type(
      return_value_, std::forward<Args>(args)...));
  output_->set_prompt(TUiMode::prompt());
  do {
    io_.run_one();
    if (aborting_) {
      throw PlayerUnavailableError();
    }
  } while (return_value_.empty());
  return_type returnValue = boost::any_cast<return_type>(return_value_);
  mode_.reset();
  parser_->pre_checker().reset();
  output_->set_prompt(prompt_);
  return_value_ = boost::any();
  return returnValue;
}

}}

#endif // KONIG_TERMINAL_COMMANDHANDLER_HPP

