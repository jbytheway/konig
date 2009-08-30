#include <konig/client/serverinterface.hpp>

#include "readlinewrapper.hpp"

int main()
{
  if (NULL == setlocale(LC_CTYPE, "")) {
    throw std::runtime_error("error setting locale");
  }
  boost::filesystem::path home(getenv("HOME"));

  boost::asio::io_service io;
  konig::client::ServerInterface si(io);
  konig::dterm::ReadlineWrapper(io, si, home/".konig"/"dterm"/"history");
  io.run();
}

