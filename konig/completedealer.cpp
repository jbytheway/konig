#include <konig/completedealer.hpp>

#include <boost/random/random_number_generator.hpp>

namespace konig {

CompleteDealer::CompleteDealer()
{
  Card::make_deck(back_inserter(deck_));
}

Deal CompleteDealer::deal()
{
  boost::random_number_generator<random_engine_type> rng(random_engine_);
  std::random_shuffle(deck_.begin(), deck_.end(), rng);

  std::vector<Card>::iterator position = deck_.begin();
  typedef std::array<Cards, 6> Chunks;
  Chunks chunks;
  Chunks::iterator next_chunk = chunks.begin();

  for (int i=0; i<6; ++i) {
    int num_cards = (i<4) ? 12 : 3;
    std::copy_n(
        position, num_cards, inserter(*next_chunk, next_chunk->begin())
      );
    position += num_cards;
    ++next_chunk;
  }
  assert(position == deck_.end());
  assert(next_chunk == chunks.end());
  return Deal(chunks);
}

}

