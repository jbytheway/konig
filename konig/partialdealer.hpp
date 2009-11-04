#ifndef KONIG__PARTIALDEALER_HPP
#define KONIG__PARTIALDEALER_HPP

#include <konig/dealer.hpp>
#include <konig/partialcards.hpp>

namespace konig {

class PartialDealer : public Dealer {
  public:
    PartialDealer(const std::string descriptions[6]);
    PartialDealer(const std::string descriptions[6], unsigned long seed);

    virtual Deal deal();
  private:
    void init(const std::string descriptions[6]);

    std::array<PartialCards, 6> partial_chunks_;
    std::vector<Card> remaining_cards_;
};

}

#endif // KONIG__PARTIALDEALER_HPP

