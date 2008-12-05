#include <konig/client/serverinterface.hpp>

#include "readlinewrapper.hpp"

int main()
{
  boost::asio::io_service io;
  konig::client::ServerInterface si(io);
  konig::dterm::ReadlineWrapper(io, si);
  io.run();
}

