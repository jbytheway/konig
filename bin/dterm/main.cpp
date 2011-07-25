#include <relasio/readline.hpp>

#include <konig/client/serverinterface.hpp>

#include "commandhandler.hpp"
#include "relasiomessagesink.hpp"

int main()
{
  if (NULL == setlocale(LC_CTYPE, "")) {
    throw std::runtime_error("error setting locale");
  }
  boost::filesystem::path home(getenv("HOME"));

  boost::asio::io_service io;
  using konig::dterm::CommandHandler;
  using std::placeholders::_1;
  using std::placeholders::_2;
  CommandHandler ch(io);
  relasio::readline rw(
    io,
    relasio::_command_handler=std::bind(&CommandHandler::command, &ch, _1),
    relasio::_eof_handler=std::bind(&CommandHandler::end, &ch),
    relasio::_history_file=home/".konig"/"dterm"/"history",
    relasio::_history_filter=[](std::string const& s){return s.size()>2;}
  );
  konig::dterm::RelasioMessageSink messageSink(rw);
  ch.set_output(messageSink);
  konig::client::ServerInterface si(io, ch);
  ch.set_server_interface(si);
  io.run();
  ch.unset_server_interface();
  ch.unset_output();
}

