#include <konig/terminal/relasiomessagesink.hpp>

#include "commandhandler.hpp"

int main()
{
  boost::filesystem::path home(getenv("HOME"));

  boost::asio::io_service io;
  using konig::real::CommandHandler;
  using std::placeholders::_1;
  using std::placeholders::_2;
  CommandHandler ch;
  relasio::readline rw(
    io,
    relasio::_command_handler=std::bind(&CommandHandler::command, &ch, _1),
    relasio::_eof_handler=std::bind(&CommandHandler::end, &ch),
    relasio::_history_file=home/".konig"/"dterm"/"history",
    relasio::_history_filter=[](std::string const& s){return s.size()>2;}
  );
  konig::terminal::RelasioMessageSink messageSink(rw);
  ch.set_output(messageSink);
  io.run();
  ch.unset_output();
}

