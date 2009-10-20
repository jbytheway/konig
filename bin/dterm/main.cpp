#include <konig/client/serverinterface.hpp>

#include "commandhandler.hpp"
#include "readlinewrapper.hpp"

int main()
{
  if (NULL == setlocale(LC_CTYPE, "")) {
    throw std::runtime_error("error setting locale");
  }
  boost::filesystem::path home(getenv("HOME"));

  boost::asio::io_service io;
  konig::dterm::CommandHandler ch(io);
  konig::dterm::ReadlineWrapper rw(io, ch, home/".konig"/"dterm"/"history");
  ch.set_output(rw);
  konig::client::ServerInterface si(io, ch);
  ch.set_server_interface(si);
  io.run();
  ch.unset_server_interface();
  ch.unset_output();
}

