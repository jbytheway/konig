#include "readlinewrapper.hpp"

#include <iostream>

#include <boost/asio/placeholders.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

#include <readline/readline.h>
#include <readline/history.h>

#include <konig/fatal.hpp>

namespace konig { namespace dterm {

// static data for callback
static ReadlineWrapper* wrapper;

static void line_callback_handler(char* line)
{
  assert(wrapper);
  wrapper->line(line);
}

ReadlineWrapper::ReadlineWrapper(
    asio::io_service& io,
    CommandHandler& ch,
    const boost::filesystem::path& hf
  ) :
  io_(io),
  timer_(io, boost::posix_time::milliseconds(10)),
  command_handler_(ch),
  history_file_(hf),
  eof_(false),
  interrupted_(false)
{
  if (wrapper) {
    throw std::logic_error("Can't install multiple ReadlineWrappers");
  }
  wrapper = this;
  rl_callback_handler_install("> ", line_callback_handler);
  if (boost::filesystem::exists(history_file_)) {
    if (!boost::filesystem::is_directory(history_file_)) {
      if (0 == read_history(history_file_.c_str())) {
        if (0 == history_set_pos(history_length)) {
          std::cerr << "error setting history position\n";
        }
        if (history_length > 0) {
          // Set up last_line_
          HIST_ENTRY* last = history_get(history_length);
          if (last) last_line_ = last->line;
        }
      } else {
        std::cerr << "error reading history\n";
      }
    }
  }
  timer_.async_wait(boost::bind(
        &ReadlineWrapper::timer_expired, this, boost::asio::placeholders::error
      ));
}

ReadlineWrapper::~ReadlineWrapper()
{
  assert(wrapper == this);
  boost::filesystem::create_directories(history_file_.parent_path());
  if (0 == write_history(history_file_.c_str())) {
    if (0 != history_truncate_file(
          history_file_.c_str(), history_length
        )) {
      std::cerr << "error truncating history\n";
    }
  } else {
    std::cerr << "error writing history\n";
  }
  rl_callback_handler_remove();
  wrapper = NULL;
  std::cout << std::endl;
}

void ReadlineWrapper::line(char* l)
{
  if (l == NULL) {
    std::cout << std::endl;
    eof_ = true;
  } else {
    if (*l && strlen(l) > 2 && l != last_line_) {
      add_history(l);
      last_line_ = l;
    }
    command_buffer_.push_back(l);
    free(l);
  }
}

void ReadlineWrapper::message(std::string const& message)
{
  poll();
  std::cout << '\n' << message << std::endl;
  rl_on_new_line();
  rl_redisplay();
}

void ReadlineWrapper::set_prompt(std::string const& prompt)
{
  rl_set_prompt(prompt.c_str());
  std::cout << std::endl;
  rl_on_new_line();
  rl_redisplay();
}

void ReadlineWrapper::interrupt()
{
  interrupted_ = true;
  timer_.cancel();
}

void ReadlineWrapper::timer_expired(boost::system::error_code const& e)
{
  if (e || interrupted_) {
    message("Readline poller interrupted");
  } else {
    poll();
    timer_.expires_from_now(boost::posix_time::milliseconds(10));
    timer_.async_wait(boost::bind(
          &ReadlineWrapper::timer_expired, this,
          boost::asio::placeholders::error
        ));
    while (!command_buffer_.empty()) {
      command_handler_.command(command_buffer_.front());
      command_buffer_.pop_front();
    }
    if (eof_) {
      command_handler_.end();
      eof_ = false;
    }
  }
}

void ReadlineWrapper::poll()
{
  int const infd = 0 /*stdin*/;
  fd_set inSet;
  FD_ZERO(&inSet);
  FD_SET(infd, &inSet);
  struct timeval timeout = {0, 0};
  while(true) {
    switch(select(infd+1, &inSet, NULL, NULL, &timeout)) {
      case -1:
        KONIG_FATAL("select failed , errno=" << errno << " (" <<
          boost::system::error_code(
            errno, boost::system::get_system_category()
          ) << ")");
      case 0:
        /* Nothing further */
        return;
      case 1:
        /* There is something to read */
        rl_callback_read_char();
        break;
      default:
        KONIG_FATAL("Unexpected return value from select.");
    }
  }
}

}}

