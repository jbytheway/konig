#include "commandhandler.hpp"

#include <list>

#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace terminal {

CommandHandler::CommandHandler() :
  output_(NULL),
  parser_(new CommandParser(*this))
{}

CommandHandler::~CommandHandler() = default;

void CommandHandler::set_output(terminal::MessageSink& output)
{
  output_ = &output;
}

void CommandHandler::unset_output()
{
  output_ = NULL;
}

void CommandHandler::command(std::string const& c)
{
  if (c.empty()) return;
  assert(output_);
#if 0
  std::vector<std::string> tokens;
  // Requires Spirit 2.1 (i.e. Boost 1.41), which isn't in Gentoo yet
  // (This code is also not finished)
  using namespace boost::spirit;
  using namespace boost::spirit::qi;
  typedef std::string::const_iterator it;
  qi::rule<it, std::string(), qi::ascii::space_type> unquoted =
    (~char_("\"'"))[_val += qi::_1] >> *(~char_(" \t\n\r\v\f"))[_val += qi::_1];
  it first = c.begin();
  bool r = phrase_parse(
      first, c.end(),
      *(
        unquoted
      ),
      qi::ascii::space,
      tokens
    );
  if (!r) {
    output_->message("error parsing command");
    return;
  }
  std::list<std::string> tokenList;
  std::move(tokens.begin(), tokens.end(), std::back_inserter(tokenList));
#else
  // Stop-gap parser until above usable
  std::list<std::string> tokenList;
  boost::algorithm::split(
      tokenList, c, boost::implicit_cast<int (*)(int)>(&std::isspace),
      boost::algorithm::token_compress_on
    );
  while (!tokenList.empty() && tokenList.front().empty()) {
    tokenList.pop_front();
  }
  while (!tokenList.empty() && tokenList.back().empty()) {
    tokenList.pop_back();
  }
  BOOST_FOREACH(std::string& token, tokenList) {
    if (token.empty()) {
      KONIG_FATAL("unexpected empty token");
    }
    if (token.size() == 1) continue;
    if (token[0] == '"' && *boost::prior(token.end()) == '"') {
      token.erase(token.begin());
      token.erase(boost::prior(token.end()));
    }
  }
#endif
  while (!tokenList.empty()) {
    auto semicolon = std::find(tokenList.begin(), tokenList.end(), ";");
    std::list<std::string> subcommand;
    subcommand.splice(
        subcommand.begin(), tokenList, tokenList.begin(), semicolon
      );
    parser_->command(std::move(subcommand));
    // Strip the semicolon
    if (!tokenList.empty()) tokenList.pop_front();
  }
}

void CommandHandler::end()
{
  assert(output_);
  output_->message("Exit");
  output_->interrupt();
}

}}

