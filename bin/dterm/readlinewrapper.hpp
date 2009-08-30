#ifndef KONG_DTERM__READLINEWRAPPER_HPP
#define KONG_DTERM__READLINEWRAPPER_HPP

#include <boost/asio/io_service.hpp>
#include <boost/filesystem/path.hpp>

#include <konig/client/serverinterface.hpp>

namespace konig { namespace dterm {

class ReadlineWrapper {
  public:
    ReadlineWrapper(
        asio::io_service& io,
        client::ServerInterface& si,
        const boost::filesystem::path& hf
      );
    ~ReadlineWrapper();
    void line(char const*);
  private:
    asio::io_service& io_;
    client::ServerInterface& server_interface_;
    boost::filesystem::path history_file_;
};

}}

#endif // KONG_DTERM__READLINEWRAPPER_HPP

