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
    num_deals(1)
  {}
  std::vector<std::string> ais;
  std::vector<std::string> chunks;
  unsigned long num_deals;
};

void usage()
{
  std::cerr << "Usage message under construction" << std::endl;
}

}}

int main(int argc, char const* const* const argv) {
  konig::simulator::Options options;
  optimal::OptionsParser parser;
  parser.addOption("ais",       'a', &options.ais,    ",");
  parser.addOption("chunks",    'h', &options.chunks, ",");
  parser.addOption("num-deals", 'n', &options.num_deals);

  boost::filesystem::path options_file(std::getenv("HOME"));
  options_file /= ".konig";
  options_file /= "simulator";
  options_file /= "config";

  if (parser.parse(options_file, argc, argv)) {
    std::cerr << "options parsing failed:\n" <<
      boost::algorithm::join(parser.getErrors(), "\n") << '\n' << std::endl;
    konig::simulator::usage();
    return EXIT_FAILURE;
  }

  std::vector<konig::ai::Ai::Ptr> ais;
  BOOST_FOREACH(const std::string& ai_desc, options.ais) {
    ais.push_back(konig::ai::Ai::create(ai_desc));
  }

  while (ais.size() < 4) {
    ais.push_back(konig::ai::Ai::create(""));
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
    konig::Game game(rules, ais, dealer->deal());
    game.play();
  }
}

