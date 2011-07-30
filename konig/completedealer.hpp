#ifndef KONIG_COMPLETEDEALER_HPP
#define KONIG_COMPLETEDEALER_HPP

#include <vector>

#include <konig/dealer.hpp>

namespace konig {

class CompleteDealer : public Dealer {
  public:
    CompleteDealer();
    CompleteDealer(unsigned long seed);

    virtual Deal deal();
  private:
    std::vector<Card> deck_;
};

}

#endif // KONIG_COMPLETEDEALER_HPP

