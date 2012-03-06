#include <vector>
#include <string>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/algorithm/string/join.hpp>

#include <optimal/optionsparser.hpp>

#include <konig/utility/split.hpp>
#include <konig/play_game.hpp>
#include <konig/dealer.hpp>
#include <konig/ai/ai.hpp>

namespace konig { namespace simulator {

struct Options {
  Options() :
    show_scores(false),
    show_deal(true),
    on_fly(false),
    gaps(false),
    help(false),
    machine(false),
    num_deals(1),
    play(true),
    show_results(true),
    show_tricks(true),
    summary(false)
  {}
  bool show_scores;
  std::vector<std::string> ais;
  std::vector<std::string> chunks;
  boost::filesystem::path chunks_file;
  bool show_deal;
  bool on_fly;
  bool gaps;
  bool help;
  bool machine;
  unsigned long num_deals;
  bool play;
  bool show_results;
  boost::optional<unsigned long> seed;
  bool show_tricks;
  bool summary;
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
"  -C, --chunks-file FILE\n"
"                File of deals to be handled in turn.  Each line should be\n"
"                a seed, space, then chunks as for --chunks.\n"
"  -d-, --no-show-deal\n"
"                Don't show the deal.\n"
"  -f, --on-fly  Show contract and deal as they happen (rather than waiting\n"
"                for the hand to end).\n"
"  -g, --gaps    Print an extra two newlines after each hand.\n"
"  -h, --help    Display this message.\n"
"  -m, --machine Use machine-readable output rather than human-readable.\n"
"  -n, --num-deals N\n"
"                Make N random deals in total (default 1).\n"
"  -p-, --no-play\n"
"                Don't play, just deal.\n"
"  -r-, --no-show-results\n"
"                Don't show the results.\n"
"  -s, --seed N  Seed dealer with N (default seeds from /dev/urandom).\n"
"  -t-, --no-show-tricks\n"
"                Don't show the tricks.\n"
"  -u, --summary Show a one-line summary of the games with average score and\n"
"                frequency of each outcome.\n"
  << std::flush;
}

Options get_options(int const argc, char const* const* const argv) {
  Options options;
  optimal::OptionsParser parser;
  parser.addOption("scores",       '#', &options.show_scores);
  parser.addOption("ais",          'a', &options.ais,    ",");
  parser.addOption("chunks",       'c', &options.chunks, ",");
  parser.addOption("chunks-file",  'C', &options.chunks_file);
  parser.addOption("show-deal",    'd', &options.show_deal);
  parser.addOption("on-fly",       'f', &options.on_fly);
  parser.addOption("gaps",         'g', &options.gaps);
  parser.addOption("help",         'h', &options.help);
  parser.addOption("machine",      'm', &options.machine);
  parser.addOption("num-deals",    'n', &options.num_deals);
  parser.addOption("play",         'p', &options.play);
  parser.addOption("show-results", 'r', &options.show_results);
  parser.addOption("seed",         's', &options.seed);
  parser.addOption("show-tricks",  't', &options.show_tricks);
  parser.addOption("summary",      'u', &options.summary);

  boost::filesystem::path options_file(std::getenv("HOME"));
  options_file /= ".konig";
  options_file /= "simulator";
  options_file /= "config";

  if (parser.parse(options_file, argc, argv)) {
    std::cerr << "options parsing failed:\n" <<
      boost::algorithm::join(parser.getErrors(), "\n") << '\n' << std::endl;
    usage(std::cerr);
    exit(EXIT_FAILURE);
  }

  if (options.help) {
    usage(std::cout);
    exit(EXIT_SUCCESS);
  }

  return options;
}

}}

int main(int argc, char const* const* const argv) {
  auto options = konig::simulator::get_options(argc, argv);

  std::vector<konig::ai::Ai::Ptr> ais;
  BOOST_FOREACH(const std::string& ai_desc, options.ais) {
    ais.push_back(konig::ai::Ai::create(ai_desc));
  }

  while (ais.size() < 4) {
    ais.push_back(konig::ai::Ai::create(""));
    assert(ais.back());
  }

  std::ostream* const debug_stream = ( options.on_fly ? &std::cout : NULL );
  konig::Ruleset const rules = konig::Ruleset::cheltenham();

  std::vector<
    std::pair<boost::optional<unsigned long>, std::vector<std::string>>
  > chunkss;

  if (!options.chunks_file.empty()) {
    boost::filesystem::ifstream is(options.chunks_file);
    std::string line;

    while (std::getline(is, line)) {
      auto space = boost::range::find(line, ' ');
      if (space == line.end()) {
        std::cerr << "malformed line in chunks file\n";
        return EXIT_FAILURE;
      }
      auto seed = boost::lexical_cast<unsigned long>(
        std::string(line.begin(), space));
      auto chunks_s = std::string(space+1, line.end());
      auto chunks = konig::utility::split(chunks_s, ',');
      chunkss.push_back({seed, chunks});
    }
  } else {
    chunkss.push_back({options.seed, options.chunks});
  }

  for (auto const& seed_chunks : chunkss) {
    auto const seed = seed_chunks.first;
    auto const& chunks_orig = seed_chunks.second;
    auto chunks = chunks_orig;

    if (chunks.size() > 6) {
      std::cerr << "too many chunks specified" << std::endl;
      return EXIT_FAILURE;
    }

    while (chunks.size() < 6) {
      chunks.push_back(std::string());
    }

    konig::Dealer::Ptr dealer = seed ?
      konig::Dealer::create(chunks, *seed) :
      konig::Dealer::create(chunks);
    std::map<std::string, unsigned long> outcome_counts;
    konig::Score total_score{0};

    for (unsigned long i=0; i<options.num_deals; ++i) {
      konig::Deal deal = dealer->deal();
      if (options.show_deal) {
        deal.write(std::cout, ( options.machine ? "," : "\n" ));
        std::cout << std::endl;
      }
      if (!options.play) continue;
      auto result = play_game(rules, ais, deal, debug_stream);

      total_score += result.scores[0];
      outcome_counts.insert({result.outcome.string(), 0}).first->second++;

      if (options.show_results) {
        std::cout << result.outcome << '\n';
      }
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
      if (options.gaps) {
        std::cout << "\n\n";
      }
    }

    if (options.summary) {
      auto chunks_s = boost::algorithm::join(chunks_orig, ",");
      std::cout <<
        (seed ? (boost::format("%03d") % *seed) : boost::format("---")) <<
        boost::format(" %s %1.6f") % chunks_s %
        (total_score/double(options.num_deals));
      BOOST_FOREACH(auto const& p, outcome_counts) {
        std::cout << ' ' << p.second << ' ' << p.first;
      }
      std::cout << std::endl;
    }
  }
}

