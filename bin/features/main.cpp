#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>

#include <optimal/optionsparser.hpp>

#include <konig/utility/split.hpp>
#include <konig/ai/feature.hpp>

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

template<typename Stream>
int process_stream(Stream&& is)
{
  std::string line;

  auto features = konig::ai::Feature::default_feature_sequence();

  std::cout << "name";

  for (auto const& fp : features) {
    std::cout << ' ' << fp->name();
  }

  std::cout << std::endl;

  while (std::getline(is, line)) {
    auto space = boost::range::find(line, ' ');
    if (space == line.end()) {
      std::cerr << "malformed line in chunks file:\n" << line << std::endl;
      return EXIT_FAILURE;
    }
    auto name = std::string(line.begin(), space);
    auto chunks = std::string(space+1, line.end());
    auto chunks_split = utility::split(chunks, ',');
    std::vector<Cards> chunks_cards;
    boost::range::transform(
      chunks_split, std::back_inserter(chunks_cards),
      px::bind(&konig::Cards::from_string, arg1)
    );

    if (chunks_cards.size() != 1 && chunks_cards.size() != 3) {
      std::cerr << "line should give 1 or 3 chunks\n" << std::endl;
      return EXIT_FAILURE;
    }

    Cards const& hand = chunks_cards[0];
    Cards discard;
    Cards rejected_half;

    if (chunks_cards.size() == 3) {
      discard = chunks_cards[1];
      rejected_half = chunks_cards[2];
    }

    std::cout << name;

    for (auto const& fp : features) {
      std::cout << ' ' << fp->compute(hand, discard, rejected_half);
    }

    std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}

}}

int main(int argc, char const* const* const argv) {
  auto options = konig::features::get_options(argc, argv);

  if (options.hands_file == "-") {
    return konig::features::process_stream(std::cin);
  } else {
    return konig::features::process_stream(
      boost::filesystem::ifstream(options.hands_file)
    );
  }
}

