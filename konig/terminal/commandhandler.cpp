#include "commandhandler.hpp"

#include <list>

#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>

#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <boost/spirit/home/phoenix/stl/container.hpp>
#include <boost/spirit/include/qi.hpp>

#include <konig/fatal.hpp>

namespace konig { namespace terminal {

CommandHandler::CommandHandler(
  boost::asio::io_service& io,
  std::string prompt
) :
  io_(io),
  output_(NULL),
  prompt_(std::move(prompt)),
  parser_(new CommandParser(*this)),
  aborting_(false)
{}

CommandHandler::~CommandHandler() = default;

void CommandHandler::set_output(terminal::MessageSink& output)
{
  output_ = &output;
  output_->set_prompt(prompt_);
}

void CommandHandler::unset_output()
{
  output_ = NULL;
}

void CommandHandler::command(std::string const& c)
{
  if (c.empty()) return;
  assert(output_);

  std::vector<std::string> tokens;
  using namespace boost::spirit;
  using namespace boost::spirit::qi;
  typedef std::string::const_iterator it;
  qi::rule<it, std::string()> unquoted =
    +(~char_("\"' \t\n\r\v\f"))[_val += qi::_1];
  qi::rule<it, std::string()> quoted_double =
    char_('"') >> *(~char_('"'))[_val += qi::_1] >> char_('"');
  qi::rule<it, std::string()> quoted_single =
    char_("'") >> *(~char_("'"))[_val += qi::_1] >> char_("'");
  it first = c.begin();
  bool r = parse(
      first, c.end(),
      *(
        qi::omit[*qi::ascii::space] >>
        (unquoted | quoted_double | quoted_single)
      ) >> qi::omit[*qi::ascii::space],
      tokens
    );
  if (!r || first != c.end()) {
    output_->message("error parsing command");
    return;
  }
  std::list<std::string> tokenList;
  std::move(tokens.begin(), tokens.end(), std::back_inserter(tokenList));

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
  aborting_ = true;
  output_->message("Exit");
  output_->interrupt();
}

}}

