#ifndef KONG_DTERM__READLINEWRAPPER_HPP
#define KONG_DTERM__READLINEWRAPPER_HPP

#include <boost/asio/io_service.hpp>

#include <konig/client/serverinterface.hpp>

namespace konig { namespace dterm {

class ReadlineWrapper {
  public:
    ReadlineWrapper(asio::io_service& io, client::ServerInterface& si);
  private:
    asio::io_service& io_;
    client::ServerInterface& server_interface_;
};

}}

#endif // KONG_DTERM__READLINEWRAPPER_HPP

