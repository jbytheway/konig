#include <konig/partialdealer.hpp>

#include <boost/random/random_number_generator.hpp>

namespace konig {

PartialDealer::PartialDealer(const std::string descriptions[6])
{
  init(descriptions);
}

PartialDealer::PartialDealer(
    const std::string descriptions[6],
    unsigned long seed
  ) :
  Dealer(seed)
{
  init(descriptions);
}

Deal PartialDealer::deal()
{
  boost::random_number_generator<random_engine_type> rng(random_engine_);
  std::random_shuffle(remaining_cards_.begin(), remaining_cards_.end(), rng);

  std::vector<Card>::iterator position = remaining_cards_.begin();
  typedef std::array<Cards, 6> Chunks;
  Chunks chunks;
  Chunks::iterator next_chunk = chunks.begin();

  BOOST_FOREACH(const PartialCards& partial, partial_chunks_) {
    position = partial.complete(position, *next_chunk);
    ++next_chunk;
  }
  assert(position == remaining_cards_.end());
  assert(next_chunk == chunks.end());
  return Deal(chunks);
}

void PartialDealer::init(const std::string descriptions[6])
{
  Cards deck = Cards::make_deck();
  for (size_t i=0; i<6; ++i) {
    size_t size;
    if (i<4) size = 12; else size = 3;
    partial_chunks_[i] = PartialCards(descriptions[i], size);
    partial_chunks_[i].erase_certainties(deck);
  }
  remaining_cards_.assign(deck.begin(), deck.end());
}

}

