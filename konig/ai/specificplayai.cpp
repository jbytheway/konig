#include <konig/ai/specificplayai.hpp>

#include <random>

#include <konig/utility/split.hpp>
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

SpecificPlayAi::SpecificPlayAi(const std::string& play_sequence)
{
  init_play_rules(play_sequence);
}

SpecificPlayAi::SpecificPlayAi(const std::vector<Card>& play_sequence)
{
  init_play_rules(play_sequence);
}

void SpecificPlayAi::reset(FateAi const&)
{
}

Card SpecificPlayAi::play_card(FateAi const& ai)
{
  size_t current_trick = ai.tricks().size() - 1;
  assert(current_trick < 12);
  Cards legal_plays = ai.legal_plays();
  BOOST_FOREACH(const PlayRule::Ptr& p, play_rules_[current_trick]) {
    Card c;
    if (p->apply(ai.tricks()[current_trick], legal_plays, c)) {
      return c;
    }
  }
  // All instructions failed; play randomly
  std::uniform_int_distribution<size_t> card_chooser(0, legal_plays.size()-1);
  size_t which = card_chooser(random_engine_);
  Cards::iterator i = legal_plays.begin();
  std::advance(i, which);
  return *i;
}

void SpecificPlayAi::init_play_rules(std::vector<Card> const& play_sequence)
{
  assert(play_sequence.size() == 12);
  BOOST_FOREACH(Card const& card, play_sequence) {
    PlayRule::Ptr p(new PlayCard(card));
    play_rules_.emplace_back();
    play_rules_.back().push_back(std::move(p));
  }
}

void SpecificPlayAi::init_play_rules(std::string const& play_sequence)
{
  // Special case: when nothing is specified, play randomly
  if (play_sequence.empty()) {
    // Amusingly the {12, {}} constructor doesn't work because it tries to copy
    // something uncopyable
    play_rules_.clear();
    for (int i=0; i<12; ++i) {
      play_rules_.emplace_back();
    }
    return;
  }

  auto chunks = konig::utility::split(
    play_sequence, ' ',
    boost::algorithm::token_compress_on
  );
  if (chunks.size() != 12) {
    throw AiError("invalid play sequence");
  }
  BOOST_FOREACH(const std::string& chunk, chunks) {
    auto play_instructions = konig::utility::split(
        chunk, ':', boost::algorithm::token_compress_on
      );
    play_rules_.push_back({});
    BOOST_FOREACH(const std::string& play_instruction, play_instructions) {
      PlayRule::Ptr p;
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

