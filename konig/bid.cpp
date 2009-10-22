#include <konig/bid.hpp>

#include <konig/fatal.hpp>

namespace konig {

Bid Bid::pass(-1);

Bid::Bid(value_type val) :
  value_(val)
{
  if (val < -1) {
    KONIG_FATAL("invalid bid");
  }
}

}

