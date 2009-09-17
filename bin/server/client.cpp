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

void Client::message(const Message<MessageType::setPlayerProperties>&)
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

}}

