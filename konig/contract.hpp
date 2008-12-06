#ifndef KONIG__CONTRACT_HPP
#define KONIG__CONTRACT_HPP

#include <boost/shared_ptr.hpp>

#include <konig/outcome.hpp>

namespace konig {

class Contract {
  public:
    typedef boost::shared_ptr<Contract> Ptr;

    virtual Outcome play() = 0;

    static Ptr solodreier();
};

}

#endif // KONIG__CONTRACT_HPP

