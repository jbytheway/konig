#include <vector>
#include <string>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>

#include <optimal/optionsparser.hpp>

#include <konig/game.hpp>
#include <konig/dealer.hpp>
#include <konig/ai/ai.hpp>

namespace konig { namespace simulator {

struct Options {
  Options() :
    show_scores(false),
    help(false),
    machine(false),
    num_deals(1),
    play(true),
    on_fly(false),
    show_deal(true),
    show_tricks(true)
  {}
  bool show_scores;
  bool help;
  std::vector<std::string> ais;
  std::vector<std::string> chunks;
  bool machine;
  unsigned long num_deals;
  boost::optional<unsigned long> seed;
  bool play;
  bool on_fly;
  bool show_deal;
  bool show_tricks;
};

void usage(std::ostream& o)
{
  o <<
"Usage : konig-simulator [OPTIONS]\n"
"\n"
"  -#, --scores  Show the scores for each hand.\n"
"  -a, --ais AI,AI,...\n"
"                Comma-separated list of AI specifications.\n"
"  -c, --chunks CHUNK,...\n"
"                Comma-separated list of partial specifications for the\n"
"                chunks of cards (4 hands, 2 talon-halves).\n"
"  -d-, --no-show-deal\n"
"                Don't show the deal.\n"
"  -f, --on-fly  Show contract and deal as they happen (rather than waiting\n"
"                for the hand to end).\n"
"  -h, --help    Display this message.\n"
"  -m, --machine Use machine-readable output rather than human-readable.\n"
"  -n, --num-deals N\n"
"                Make N random deals in total (default 1).\n"
"  -p-, --no-play\n"
"                Don't play, just deal.\n"
"  -s, --seed N  Seed dealer with N (default seeds from /dev/urandom).\n"
"  -t-, --no-show-tricks\n"
"                Don't show the tricks.\n"
  << std::flush;
}

}}

int main(int argc, char const* const* const argv) {
  konig::simulator::Options options;
  optimal::OptionsParser parser;
  parser.addOption("scores",      '#', &options.show_scores);
  parser.addOption("ais",         'a', &options.ais,    ",");
  parser.addOption("chunks",      'c', &options.chunks, ",");
  parser.addOption("show-deal",   'd', &options.show_deal);
  parser.addOption("on-fly",      'f', &options.on_fly);
  parser.addOption("help",        'h', &options.help);
  parser.addOption("machine",     'm', &options.machine);
  parser.addOption("num-deals",   'n', &options.num_deals);
  parser.addOption("play",        'p', &options.play);
  parser.addOption("seed",        's', &options.seed);
  parser.addOption("show-tricks", 't', &options.show_tricks);

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

  std::ostream* debug_stream = ( options.on_fly ? &std::cout : NULL );
  konig::Ruleset rules = konig::Ruleset::cheltenham();
  konig::Dealer::Ptr dealer = options.seed ?
    konig::Dealer::create(options.chunks, *options.seed) :
    konig::Dealer::create(options.chunks);
  for (unsigned long i=0; i<options.num_deals; ++i) {
    konig::Deal deal = dealer->deal();
    if (options.show_deal) {
      deal.write(std::cout, ( options.machine ? "," : "\n" ));
      std::cout << std::endl;
    }
    if (!options.play) continue;
    konig::Game game(rules, ais, deal);
    auto result = game.play(debug_stream);
    std::cout << result.outcome << '\n';
    if (options.show_scores) {
      std::copy(
        result.scores.begin(), result.scores.end(),
        std::ostream_iterator<int>(std::cout, " ")
      );
      std::cout << '\n';
    }
    if (options.show_tricks) {
      std::cout << '\n';
      std::copy(
          result.tricks.begin(), result.tricks.end(),
          std::ostream_iterator<konig::Trick>(std::cout, "\n")
        );
    }
  }
}

