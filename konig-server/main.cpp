#include <messaging/server.hpp>

#include "server.hpp"

int main()
{
  boost::asio::io_service io;
  konig::server::Server server(io);
  io.run();
}

