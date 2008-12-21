#ifndef KONIG__DEALER_HPP
#define KONIG__DEALER_HPP

#include <random>

#include <boost/shared_ptr.hpp>

#include <konig/deal.hpp>

namespace konig {

class Dealer {
  public:
    typedef boost::shared_ptr<Dealer> Ptr;

    template<typename ForwardRange>
    static Ptr create(const ForwardRange& descriptions);

    // Argument must be an array reference to be used instead of the templated
    // overload
    static Ptr create(const std::string (& descriptions)[6]);

    Dealer();

    virtual Deal deal() = 0;
  protected:
    typedef std::mt19937 random_engine_type;
    random_engine_type random_engine_;
};

template<typename ForwardRange>
Dealer::Ptr Dealer::create(const ForwardRange& descriptions)
{
  if (boost::size(descriptions) != 6) {
    throw std::logic_error("wrong number of chunks in desription");
  }

  std::string descs[6];
  std::copy(boost::begin(descriptions), boost::end(descriptions), descs);

  return create(descs);
}

}

#endif // KONIG__DEALER_HPP

