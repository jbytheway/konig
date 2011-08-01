#include <boost/algorithm/string/join.hpp>

#include <optimal/optionsparser.hpp>

#include <konig/ruleset.hpp>
#include <konig/terminal/relasiomessagesink.hpp>
#include <konig/ai/ai.hpp>

#include "commandhandler.hpp"
#include "oracle.hpp"
#include "realgame.hpp"

namespace konig { namespace real {

struct Options {
  Options() :
    help(false)
  {}

  std::string ai;
  bool help;
};

void usage(std::ostream& o)
{
  o <<
"Usage : konig-real [OPTIONS]\n"
"\n"
"  -a, --ai AI   AI specification.\n"
"  -h, --help    Display this message.\n"
  << std::flush;
}

}}

int main(int argc, char* argv[])
{
  konig::real::Options options;
  optimal::OptionsParser parser;
  parser.addOption("ai",   'a', &options.ai);
  parser.addOption("help", 'h', &options.help);

  boost::filesystem::path home(getenv("HOME"));
  boost::filesystem::path options_file = home/".konig"/"real"/"config";

  if (parser.parse(options_file, argc, argv)) {
    std::cerr << "options parsing failed:\n" <<
      boost::algorithm::join(parser.getErrors(), "\n") << '\n' << std::endl;
    konig::real::usage(std::cerr);
    return EXIT_FAILURE;
  }

  if (options.help) {
    konig::real::usage(std::cout);
    return EXIT_SUCCESS;
  }

  boost::asio::io_service io;
  using konig::real::CommandHandler;
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
  konig::terminal::RelasioMessageSink messageSink(rw);
  ch.set_output(messageSink);
  konig::Ruleset rules = konig::Ruleset::cheltenham();

  konig::ai::Ai::Ptr ai = konig::ai::Ai::create(options.ai);
  konig::real::Oracle oracle(ch, ai);

  konig::real::RealGame game(rules, oracle);
  io.post(std::bind(&konig::real::RealGame::play, game));
  io.run();
  ch.unset_output();
}

