#include "client.hpp"

#include <iostream>

#include <konig/fatal.hpp>

#include "server.hpp"

namespace konig { namespace server {

Client::~Client()
{
  connection_->reset_callbacks();
  connection_->close();
}

#define KONIG_SERVER_CLIENT_IGNORE(type)              \
void Client::message(const Message<type>&) {           \
  std::ostringstream os;                              \
  os << "warning: ignoring message of type " << type; \
  server_.warning(os.str());                          \
}
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifySetting)
#undef KONIG_SERVER_CLIENT_IGNORE

void Client::message(const Message<MessageType::setSetting>&)
{
  KONIG_FATAL("not implemented");
}

void Client::error(
    const messaging::error_source es,
    const boost::system::error_code& ec
  )
{
  std::cerr << "error: client: " << es << ": " << ec.message() << std::endl;
  server_.remove_client(shared_from_this());
}

void Client::close()
{
  connection_->close_gracefully();
}

}}

