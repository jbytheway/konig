#include <konig/ai/specificplayai.hpp>

#include <random>

#include <boost/algorithm/string/split.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>

#include <konig/ai/aierror.hpp>

namespace konig { namespace ai {

struct PlayHigh : SpecificPlayAi::PlayRule {
  virtual bool apply(const Trick& trick, const Cards& legal, Card& play) {
    if (trick.played() == 0) {
      return false;
    }
    Card highest = *boost::prior(legal.end());
    if (highest.suit() == trick.suit() || highest.suit() == Suit::trumps) {
      play = highest;
      return true;
    }
    return false;
  }
};

struct PlayCard : SpecificPlayAi::PlayRule {
  PlayCard(const Card c) : card_(c) {}

  Card card_;

  virtual bool apply(const Trick&, const Cards& legal, Card& play) {
    if (legal.count(card_)) {
      play = card_;
      return true;
    }
    return false;
  }
};

SpecificPlayAi::SpecificPlayAi(const std::string& play_sequence) :
  next_bid_(bids_.begin())
{
  init_play_rules(play_sequence);
}

SpecificPlayAi::SpecificPlayAi(
    std::vector<Bid> bids,
    const std::string& play_sequence
  ) :
  bids_(std::move(bids)),
  next_bid_(bids_.begin())
{
  init_play_rules(play_sequence);
}

Bid SpecificPlayAi::bid() {
  if (next_bid_ == bids_.end()) return Bid::pass;
  Bid b = *next_bid_;
  ++next_bid_;
  return b;
}

KingCall SpecificPlayAi::call_king() {
  throw std::logic_error("not implemented");
}

uint8_t SpecificPlayAi::choose_talon_half() {
  throw std::logic_error("not implemented");
}

Cards SpecificPlayAi::discard() {
  throw std::logic_error("not implemented");
}

std::vector<Announcement> SpecificPlayAi::announce() {
  return std::vector<Announcement>();
}

Card SpecificPlayAi::play_card() {
  size_t current_trick = tricks().size() - 1;
  assert(current_trick < 12);
  Cards legal_plays = this->legal_plays();
  BOOST_FOREACH(const PlayRule::Ptr& p, play_rules_[current_trick]) {
    Card c;
    if (p->apply(tricks()[current_trick], legal_plays, c)) {
      return c;
    }
  }
  // All instructions failed; play randomly
  std::uniform_int<size_t> card_chooser(0, legal_plays.size()-1);
  size_t which = card_chooser(random_engine_);
  Cards::iterator i = legal_plays.begin();
  std::advance(i, which);
  return *i;
}

void SpecificPlayAi::init_play_rules(std::string const& play_sequence)
{
  // Special case: when nothing is specified, play randomly
  if (play_sequence.empty()) {
    std::list<PlayRule::Ptr> p;
    play_rules_ = std::vector<std::list<PlayRule::Ptr>>(12, p);
    return;
  }

  std::vector<std::string> chunks;
  boost::algorithm::split(
      chunks, play_sequence, arg1 == ' ',
      boost::algorithm::token_compress_on
    );
  if (chunks.size() != 12) {
    throw AiError("invalid play sequence");
  }
  BOOST_FOREACH(const std::string& chunk, chunks) {
    std::vector<std::string> play_instructions;
    boost::algorithm::split(
        play_instructions, chunk, arg1 == ':',
        boost::algorithm::token_compress_on
      );
    play_rules_.push_back(std::list<boost::shared_ptr<PlayRule> >());
    BOOST_FOREACH(const std::string& play_instruction, play_instructions) {
      boost::shared_ptr<PlayRule> p;
      if (play_instruction == "h") {
        p.reset(new PlayHigh());
      } else {
        p.reset(new PlayCard(Card(play_instruction)));
      }
      play_rules_.back().push_back(std::move(p));
    }
  }
}

}}

