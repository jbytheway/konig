#include <string>
#include <iostream>

#include <boost/optional.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/asio/io_service.hpp>

#include <optionsparser.h>

#include <konig/tableposition.hpp>
#include <konig/client/serverinterface.hpp>
#include <konig/ai/ai.hpp>
#include <konig/ai/nosuchai.hpp>

#include "aiclient.hpp"

namespace konig { namespace ai_client {

struct Options {
  Options() :
    help(false),
    debug(true)
  {}
  bool help;
  bool debug;
  boost::optional<int> position;
  boost::optional<std::string> description;
};

void usage(std::ostream& o)
{
  o <<
"Usage: konig-ai_client -h\n"
"       konig-ai_client [-d] -p POSITION -a DESCRIPTION\n"
"  -d, --debug   Output more debugging information.\n"
"  -h, --help    Display this message.\n"
"  -p, --position POSITION\n"
"                Join table at position POSITION (1--4)\n"
"  -a, --ai DESCRIPTION\n"
"                Run AI described by DESCRIPTION\n"
  << std::flush;
}

}}

int main(int const argc, char const* const* const argv)
{
  konig::ai_client::Options options;
  optimal::OptionsParser parser;
  parser.addOption("help",     'h', &options.help);
  parser.addOption("debug",    'd', &options.debug);
  parser.addOption("position", 'p', &options.position);
  parser.addOption("ai",       'a', &options.description);

  if (parser.parse(argc, argv)) {
    std::cerr << "options parsing failed\n" <<
      boost::algorithm::join(parser.getErrors(), "\n") << '\n' << std::endl;
    konig::ai_client::usage(std::cerr);
    return EXIT_FAILURE;
  }

  if (options.help) {
    konig::ai_client::usage(std::cerr);
    return EXIT_SUCCESS;
  }

  if (!options.description) {
    std::cerr << "must provide AI description\n\n";
    konig::ai_client::usage(std::cerr);
    return EXIT_FAILURE;
  }

  if (!options.position) {
    std::cerr << "must provide table position\n\n";
    konig::ai_client::usage(std::cerr);
    return EXIT_FAILURE;
  }

  if (*options.position < 1 || *options.position >= konig::TablePosition::max) {
    std::cerr << "position invalid\n";
    return EXIT_FAILURE;
  }

  konig::ai::Ai::Ptr ai;

  try {
    ai = konig::ai::Ai::create(*options.description);
  } catch (konig::ai::AiError const& e) {
    std::cerr << "error creating AI: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  boost::asio::io_service io;
  konig::ai_client::AiClient client(*ai, options.debug, *options.position);
  konig::client::ServerInterface si(io, client);
  client.set_server_interface(si);
  io.run();
  client.unset_server_interface();

  return EXIT_SUCCESS;
}

