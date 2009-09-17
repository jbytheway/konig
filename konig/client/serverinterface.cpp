#include "serverinterface.hpp"

#include <messaging/create_connection.hpp>
#include <messaging/callback_helper.hpp>

#include <konig/protocol.hpp>

namespace konig { namespace client {

ServerInterface::ServerInterface(asio::io_service& io, ClientInterface& cl) :
  client_(cl)
{
  connection_ = messaging::create_connection<Protocol>(
      io, asio::ip::tcp::endpoint(
        asio::ip::address_v4::from_string("127.0.0.1"), Protocol::port
      ), messaging::callback_helper<ServerInterface>(*this)
    );
}

ServerInterface::~ServerInterface()
{
  connection_->reset_callbacks();
  connection_->close();
}

void ServerInterface::error(
    const messaging::error_source es,
    const boost::system::error_code& ec
  )
{
  std::ostringstream os;
  os << "error: " << es << ": " << ec.message();
  client_.warning(os.str());
  client_.abort();
  if (connection_) connection_->close();
}

void ServerInterface::close()
{
  connection_->close_gracefully();
}

}}

