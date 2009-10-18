#ifndef KONIG__COMPLETEDEALER_HPP
#define KONIG__COMPLETEDEALER_HPP

#include <vector>

#include <konig/dealer.hpp>

namespace konig {

class CompleteDealer : public Dealer {
  public:
    CompleteDealer();

    virtual Deal deal();
  private:
    std::vector<Card> deck_;
};

}

#endif // KONIG__COMPLETEDEALER_HPP

