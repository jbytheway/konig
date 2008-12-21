#include <vector>
#include <string>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>

#include <optionsparser.h>

#include <konig/game.hpp>
#include <konig/dealer.hpp>
#include <konig/ai/ai.hpp>

namespace konig { namespace simulator {

struct Options {
  Options() :
    help(false),
    num_deals(1)
  {}
  bool help;
  std::vector<std::string> ais;
  std::vector<std::string> chunks;
  unsigned long num_deals;
};

void usage(std::ostream& o)
{
  o <<
"Usage : konig-simulator [OPTIONS]\n"
"\n"
"  -h, --help    Display this message.\n"
"  -a, --ais AI,AI,...\n"
"                Comma-separated list of AI specifications.\n"
"  -c, --chunks CHUNK,...\n"
"                Comma-separated list of partial specifications for the\n"
"                chunks of cards (4 hands, 2 talon-halves)\n"
"  -n, --num-deals N\n"
"                Make N random deals in total (default 1).\n"
  << std::flush;
}

}}

int main(int argc, char const* const* const argv) {
  konig::simulator::Options options;
  optimal::OptionsParser parser;
  parser.addOption("help",      'h', &options.help);
  parser.addOption("ais",       'a', &options.ais,    ",");
  parser.addOption("chunks",    'c', &options.chunks, ",");
  parser.addOption("num-deals", 'n', &options.num_deals);

  boost::filesystem::path options_file(std::getenv("HOME"));
  options_file /= ".konig";
  options_file /= "simulator";
  options_file /= "config";

  if (parser.parse(options_file, argc, argv)) {
    std::cerr << "options parsing failed:\n" <<
      boost::algorithm::join(parser.getErrors(), "\n") << '\n' << std::endl;
    konig::simulator::usage(std::cerr);
    return EXIT_FAILURE;
  }

  if (options.help) {
    konig::simulator::usage(std::cout);
    return EXIT_SUCCESS;
  }

  std::vector<konig::ai::Ai::Ptr> ais;
  BOOST_FOREACH(const std::string& ai_desc, options.ais) {
    ais.push_back(konig::ai::Ai::create(ai_desc));
  }

  while (ais.size() < 4) {
    ais.push_back(konig::ai::Ai::create(""));
    assert(ais.back());
  }

  if (options.chunks.size() > 6) {
    std::cerr << "too many chunks specified" << std::endl;
    return EXIT_FAILURE;
  }

  while (options.chunks.size() < 6) {
    options.chunks.push_back(std::string());
  }

  konig::Ruleset rules = konig::Ruleset::solodreier_only();
  konig::Dealer::Ptr dealer(konig::Dealer::create(options.chunks));
  for (unsigned long i=0; i<options.num_deals; ++i) {
    konig::Deal deal = dealer->deal();
    std::cout << deal << std::endl;
    konig::Game game(rules, ais, deal);
    konig::Outcome outcome;
    std::vector<konig::Trick> tricks;
    boost::tie(outcome, tricks) = game.play();
    std::cout << outcome << "\n\n";
    std::copy(
        tricks.begin(), tricks.end(),
        std::ostream_iterator<konig::Trick>(std::cout, "\n")
      );
  }
}

