#ifndef KONG_DTERM__READLINEWRAPPER_HPP
#define KONG_DTERM__READLINEWRAPPER_HPP

#include <list>

#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/filesystem/path.hpp>

#include <konig/client/serverinterface.hpp>

#include "commandhandler.hpp"

namespace konig { namespace dterm {

class ReadlineWrapper : public MessageSink {
  public:
    ReadlineWrapper(
        asio::io_service&,
        CommandHandler&,
        const boost::filesystem::path& hf
      );
    ~ReadlineWrapper();
    void line(char*);
    virtual void message(std::string const&);
    virtual void interrupt();
  private:
    void timer_expired(boost::system::error_code const&);
    void poll();

    asio::io_service& io_;
    boost::asio::deadline_timer timer_;
    CommandHandler& command_handler_;
    boost::filesystem::path history_file_;
    std::string last_line_;
    bool eof_;
    bool interrupted_;
    std::list<std::string> command_buffer_;
};

}}

#endif // KONG_DTERM__READLINEWRAPPER_HPP

