#ifndef KONIG__PARTIALCARDS
#define KONIG__PARTIALCARDS

#include <vector>
#include <iterator>

#include <boost/foreach.hpp>

#include <konig/cards.hpp>

namespace konig {

class PartialCards {
  public:
    PartialCards() : num_random_(0) {}
    PartialCards(const std::string& description, size_t size);

    template<typename AssociativeContainer>
    void erase_certainties(AssociativeContainer& c) {
      BOOST_FOREACH(const Card& card, fixed_) {
        c.erase(card);
      }
    }

    template<typename InputIterator>
    InputIterator complete(InputIterator i, Cards& out) const {
      assert(out.empty());
      std::insert_iterator<std::set<Card> > inserter(out, out.begin());
      std::copy(fixed_.begin(), fixed_.end(), inserter);
      InputIterator end = i+num_random_;
      std::copy(i, end, inserter);
      return end;
    }
  private:
    Cards fixed_;
    size_t num_random_;
};

}

#endif // KONIG__PARTIALCARDS

