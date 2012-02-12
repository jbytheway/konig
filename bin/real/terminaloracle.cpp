#include "terminaloracle.hpp"

#include <ostream>
#include <sstream>

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/range/algorithm/copy.hpp>

#include <konig/terminal/uimodes.hpp>

#include "commandhandler.hpp"
#include "uimodes.hpp"

namespace konig { namespace real {

TerminalOracle::TerminalOracle(
  Ruleset const& rules,
  CommandHandler& handler,
  ai::Ai::Ptr ai
) :
  rules_(rules),
  handler_(handler),
  ai_(std::move(ai)),
  position_(position_max)
{
}

void TerminalOracle::init()
{
  position_ = handler_.get_from_user<uiModes::PlayPosition>();
}

Cards TerminalOracle::get_hand()
{
  return handler_.get_from_user<uiModes::Hand>();
}

std::array<Cards, 2> TerminalOracle::get_talon()
{
  std::array<Cards, 2> result;
  result[0] = handler_.get_from_user<uiModes::Talon>();
  result[1] = handler_.get_from_user<uiModes::Talon>();
  return result;
}

Cards TerminalOracle::get_ouvert(PlayPosition position)
{
  if (position == position_) {
    return ai_->hand();
  } else {
    return handler_.get_from_user<uiModes::Ouvert>();
  }
}

void TerminalOracle::start_game(Ruleset const& rules, Cards const& hand)
{
  ai_->start_game(rules, position_, hand);
}

namespace {

  template<typename Value>
  void inform(
    terminal::MessageSink& o,
    std::string const& message,
    Value const& value,
    typename boost::disable_if<std::is_integral<Value>>::type* = 0
  )
  {
    std::ostringstream os;
    os << message << value;
    o.message(os.str());
  }

  template<typename Value>
  void inform(
    terminal::MessageSink& o,
    std::string const& message,
    Value const& value,
    typename boost::enable_if<std::is_integral<Value>>::type* = 0
  )
  {
    std::ostringstream os;
    os << message << int64_t(value);
    o.message(os.str());
  }

  template<typename Value>
  void inform(
    terminal::MessageSink& o,
    std::string const& message,
    std::vector<Value> const& values
  )
  {
    std::ostringstream os;
    os << message;
    boost::range::copy(values, std::ostream_iterator<Value>(os, " "));
    o.message(os.str());
  }

}

Bid TerminalOracle::bid(PlayPosition position)
{
  if (position == position_) {
    Bid result = ai_->bid();
    inform(handler_.output(), "bid ",
      result.is_pass() ? "pass" : rules_.contracts()[result].name());
    return result;
  } else {
    return handler_.get_from_user<terminal::uiModes::Bid>(rules_);
  }
}

#define FORWARDING_MEMBER(ret, name, uiMode, message) \
ret TerminalOracle::name(PlayPosition position) \
{                                               \
  if (position == position_) {                  \
    ret result = ai_->name();                   \
    inform(handler_.output(), message, result); \
    return result;                              \
  } else {                                      \
    return handler_.get_from_user<terminal::uiModes::uiMode>(); \
  }                                             \
}

FORWARDING_MEMBER(KingCall, call_king, CallKing, "called ")
FORWARDING_MEMBER(bool, choose_concede, ChooseConcede, "conceded ")
FORWARDING_MEMBER(uint8_t, choose_talon_half, ChooseTalonHalf, "chose ")
FORWARDING_MEMBER(Cards, discard, Discard, "discarded ")
FORWARDING_MEMBER(
  std::vector<Announcement>, announce, Announce, "announcement: "
)
FORWARDING_MEMBER(Card, play_card, PlayCard, "played ")

#undef FORWARDING_MEMBER

void TerminalOracle::notify_contract_established(Bid bid)
{
  ai_->notify_contract_established(bid);
  inform(handler_.output(), "contract is ", rules_.contracts()[bid].name());
}

void TerminalOracle::notify_result(PlayResult const& result)
{
  std::ostringstream os;
  result.dump(os);
  handler_.output().message(os.str());
}
void TerminalOracle::notify_play_card(PlayPosition position, Card card)
{
  ai_->notify_play_card(position, card);
  // FIXME: This is nasty; would be much nicer if we could use
  // trick_complete_hook somehow
  auto const& tricks = ai_->tricks();
  if (tricks.size() >= 2 ) {
    Trick const& current_trick = tricks.back();
    Trick const& complete_trick = current_trick.played() == 0 ?
      *boost::prior(boost::prior(tricks.end())) : current_trick;
    if (complete_trick.complete()) {
      auto s = boost::lexical_cast<std::string>(complete_trick);
      handler_.output().message(s);
    }
  }
}

#define NOTIFY_MEMBER_PARAM(z, i, types) \
  BOOST_PP_SEQ_ELEM(i, types) a##i

#define NOTIFY_MEMBER(name, types)                       \
void TerminalOracle::notify_##name(BOOST_PP_ENUM(        \
    BOOST_PP_SEQ_SIZE(types), NOTIFY_MEMBER_PARAM, types \
))                                                       \
{                                                        \
  ai_->notify_##name(BOOST_PP_ENUM_PARAMS(               \
      BOOST_PP_SEQ_SIZE(types), a                        \
  ));                                                    \
}

NOTIFY_MEMBER(bid, (PlayPosition)(Bid))
NOTIFY_MEMBER(call_king, (KingCall))
typedef std::array<Cards, 2> TwoCards;
NOTIFY_MEMBER(talon, (TwoCards const&))
NOTIFY_MEMBER(concede, )
NOTIFY_MEMBER(talon_choice, (uint8_t))
NOTIFY_MEMBER(discard, (Cards))
NOTIFY_MEMBER(announcements, (std::vector<Announcement>))
NOTIFY_MEMBER(announcements_done, )
NOTIFY_MEMBER(ouvert, (Cards const&))

#undef NOTIFY_MEMBER
#undef NOTIFY_MEMBER_PARAM

void TerminalOracle::notify_invalid(
  PlayPosition position, std::string const& message
)
{
  handler_.output().message(message);
  if (position == position_) {
    ai_->notify_invalid(message);
  }
}

}}

