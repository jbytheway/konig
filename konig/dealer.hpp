#ifndef KONIG_DEALER_HPP
#define KONIG_DEALER_HPP

#include <random>

#include <boost/shared_ptr.hpp>

#include <konig/deal.hpp>

namespace konig {

class KONIG_API Dealer {
  public:
    typedef boost::shared_ptr<Dealer> Ptr;

    static Ptr create();
    static Ptr create(unsigned long seed);

    template<typename ForwardRange>
    static Ptr create(const ForwardRange& descriptions);

    template<typename ForwardRange>
    static Ptr create(const ForwardRange& descriptions, unsigned long seed);

    // Argument must be an array reference to be used instead of the templated
    // overload
    static Ptr create(const std::string (& descs)[6]);
    static Ptr create(const std::string (& descs)[6], unsigned long seed);

    Dealer();
    Dealer(unsigned long seed);

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

template<typename ForwardRange>
Dealer::Ptr Dealer::create(const ForwardRange& descriptions, unsigned long seed)
{
  if (boost::size(descriptions) != 6) {
    throw std::logic_error("wrong number of chunks in desription");
  }

  std::string descs[6];
  std::copy(boost::begin(descriptions), boost::end(descriptions), descs);

  return create(descs, seed);
}

}

#endif // KONIG_DEALER_HPP

