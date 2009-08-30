#include "readlinewrapper.hpp"

#include <iostream>

#include <boost/filesystem/operations.hpp>

#include <readline/readline.h>
#include <readline/history.h>

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
    client::ServerInterface& si,
    const boost::filesystem::path& hf
  ) :
  io_(io),
  server_interface_(si),
  history_file_(hf)
{
  if (wrapper) {
    throw std::logic_error("Can't install multiple ReadlineWrappers");
  }
  wrapper = this;
  rl_callback_handler_install("> ", line_callback_handler);
  if (boost::filesystem::exists(history_file_)) {
    if (!boost::filesystem::is_directory(history_file_)) {
      if (0 == read_history(history_file_.native_file_string().c_str())) {
        if (0 == history_set_pos(history_length)) {
          std::cerr << "error setting history position\n";
        }
      } else {
        std::cerr << "error reading history\n";
      }
    }
  }
}

ReadlineWrapper::~ReadlineWrapper()
{
  assert(wrapper == this);
  if (0 == write_history(history_file_.native_file_string().c_str())) {
    if (0 != history_truncate_file(
          history_file_.native_file_string().c_str(), history_length
        )) {
      std::cerr << "error truncating history\n";
    }
  } else {
    std::cerr << "error writing history\n";
  }
  rl_callback_handler_remove();
  wrapper = NULL;
}

}}

