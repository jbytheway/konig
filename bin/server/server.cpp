#include "server.hpp"

namespace konig { namespace server {

Server::Server(boost::asio::io_service& io) :
  message_server_(
      io,
      callback_helper(*this),
      asio::ip::tcp::endpoint(asio::ip::tcp::v4(), Protocol::port)
    )
{}

void Server::remove_client(const Client::Ptr& client)
{
  clients_.erase(client);
}

}}

