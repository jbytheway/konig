#include "readlinewrapper.hpp"

namespace konig { namespace dterm {

ReadlineWrapper::ReadlineWrapper(
    asio::io_service& io,
    client::ServerInterface& si
  ) :
  io_(io),
  server_interface_(si)
{
}

}}

