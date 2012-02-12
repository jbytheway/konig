#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/algorithm/find.hpp>

#include <optimal/optionsparser.hpp>

#include "feature.hpp"

namespace konig { namespace features {

struct Options {
  Options() :
    help{}
  {}

  bool help;
  boost::filesystem::path hands_file;
};

void usage(std::ostream& o)
{
  o <<
"Usage: konig-features [OPTIONS] HAND_FILE\n"
"\n"
"  Each line of HAND_FILE should be a name, space, hand.\n"
"  Output is to stdout, and computes a number of features for each hand.\n"
"\n"
"  -h, --help    Display this message.\n"
  << std::flush;
}

Options get_options(int const argc, char const* const* const argv) {
  Options options;
  std::vector<char const*> positional_args;
  optimal::OptionsParser parser(&positional_args);
  parser.addOption("help",         'h', &options.help);

  if (parser.parse(argc, argv)) {
    std::cerr << "options parsing failed:\n" <<
      boost::algorithm::join(parser.getErrors(), "\n") << '\n' << std::endl;
    usage(std::cerr);
    exit(EXIT_FAILURE);
  }

  if (positional_args.size() != 1) {
    std::cerr << "no hand file given\n";
    usage(std::cerr);
    exit(EXIT_FAILURE);
  }

  if (options.help) {
    usage(std::cout);
    exit(EXIT_SUCCESS);
  }

  options.hands_file = positional_args[0];

  return options;
}

}}

int main(int argc, char const* const* const argv) {
  auto options = konig::features::get_options(argc, argv);

  boost::filesystem::ifstream is(options.hands_file);
  std::string line;

  auto features = konig::features::Feature::default_features();

  std::cout << "name";

  for (auto const& fp : features) {
    std::cout << ' ' << fp->name();
  }

  std::cout << std::endl;

  while (std::getline(is, line)) {
    auto space = boost::range::find(line, ' ');
    if (space == line.end()) {
      std::cerr << "malformed line in chunks file\n";
      return EXIT_FAILURE;
    }
    auto name = boost::lexical_cast<unsigned long>(
      std::string(line.begin(), space));
    auto hand_s = std::string(space+1, line.end());
    auto hand = konig::Cards::from_string(hand_s);

    std::cout << name;

    for (auto const& fp : features) {
      std::cout << ' ' << fp->compute(hand);
    }

    std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}

