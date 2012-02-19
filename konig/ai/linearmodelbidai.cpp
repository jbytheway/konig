#include <konig/ai/linearmodelbidai.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include <optimal/optionsparser.hpp>

#include <konig/utility/split.hpp>
#include <konig/ai/linearpredictor.hpp>
#include <konig/ai/aierror.hpp>

namespace konig { namespace ai {

LinearModelBidAi::LinearModelBidAi(std::string const& description)
{
  boost::filesystem::path file_name;
  optimal::OptionsParser parser(':');
  parser.addOption("file", '\0', &file_name);
  std::istringstream is(description);
  if (parser.parseStream(is, "bid=linear")) {
    throw AiError(boost::algorithm::join(parser.getErrors(), "\n"));
  }

  {
    boost::filesystem::ifstream file(file_name);
    if (!file.is_open()) {
      throw AiError("couldn't open file "+file_name.string());
    }
    std::string line;
    while (getline(file, line)) {
      boost::algorithm::trim(line);
      lines_.push_back(std::move(line));
    }
  }

  if (lines_.empty()) {
    throw AiError("no lines in model file "+file_name.string());
  }

  // The file looks like this:
  //
  // contract  (Intercept)   numtrumps
  // t           3.6638461  -0.8090828
  auto headers = lines_.front();
  lines_.pop_front();
  auto field_names = konig::utility::split(
    headers, ' ', boost::algorithm::token_compress_on);
  if (field_names.size() < 3) {
    throw AiError("Too few fields for model");
  }
  if (field_names[0] != "contract") {
    throw AiError("First field should be contract");
  }
  if (field_names[1] != "(Intercept)") {
    throw AiError("Second field should be (Intercept)");
  }
  auto feature_map = Feature::default_feature_map();
  for (size_t i=2; i<field_names.size(); ++i) {
    auto feature_name = field_names[i];
    auto it = feature_map.find(feature_name);
    if (it == feature_map.end()) {
      throw AiError("No such feature "+feature_name);
    }
    features_.push_back(it->second);
  }
}

void LinearModelBidAi::reset(Ai const& ai)
{
  // Set up the predictors again
  std::map<Bid, LinearPredictor> predictors;
  auto const& contracts = ai.rules().contracts();

  for (auto const& line : lines_) {
    auto fields = utility::split(
      line, ' ', boost::algorithm::token_compress_on);
    if (fields.size() != features_.size()+2) {
      throw AiError("Wrong numer of fields");
    }
    auto const contract_name = fields[0];
    Bid const contract = contracts.index_by_bid_name(contract_name);
    auto const intercept = boost::lexical_cast<double>(fields[1]);
    LinearPredictor p(intercept);
    for (size_t i=2; i<fields.size(); ++i) {
      auto const feature = features_[i-2];
      auto const coefficient = boost::lexical_cast<double>(fields[i]);
      p.add(feature, coefficient);
    }
    predictors.insert({contract, p});
  }
  Bid const rufer = contracts.index_by_bid_name("r");
  auto it = predictors.find(rufer);
  if (it == predictors.end()) {
    throw AiError("No rufer predictor provided");
  }

  // Figure out what it is we want to bid for this time around
  auto const& hand = ai.hand();
  assert(hand.size() == 12);
  Bid best_bid = Bid::pass;
  double best_prediction{};
  for (auto const& p : predictors) {
    Bid const contract = p.first;
    auto const& predictor = p.second;

    // If not forehand we only consider rufer amongst the reserved bids
    if (ai.position() != position_forehand &&
      contract < contracts.reserved_bids() && contract != rufer) {
      continue;
    }
    double prediction = predictor.predict(hand);
    if (best_bid.is_pass() || prediction > best_prediction) {
      best_bid = contract;
      best_prediction = prediction;
    }
  }

  if (best_bid >= contracts.reserved_bids() ||
    ai.position() == position_forehand) {
    bid_ = best_bid;
  } else {
    bid_ = Bid::pass;
  }
}

Bid LinearModelBidAi::bid(Ai const& ai)
{
  auto const& contracts = ai.rules().contracts();
  if (ai.last_non_pass().is_pass() && bid_ < contracts.reserved_bids()) {
    return Bid(0);
  }
  if (bid_ >= ai.last_non_pass() &&
     (bid_ > ai.last_non_pass() || ai.position() == position_forehand)) {
    return bid_;
  }
  return Bid::pass;
}

}}

