#ifndef KONIG__DEAL_HPP
#define KONIG__DEAL_HPP

#include <boost/foreach.hpp>
#include <boost/array.hpp>
#include <boost/range/size.hpp>
#include <boost/range/value_type.hpp>

#include <konig/cards.hpp>

namespace konig {

class KONIG_API Deal {
  public:
    template<typename Range>
    explicit Deal(
        Range chunkss,
        typename boost::enable_if<
            typename std::is_same<
              typename boost::range_value<Range>::type,
              Cards
            >::type,
            int
          >::type = 0
      );

    template<typename Range1, typename Range2>
    Deal(Range1 hands, Range2 talon);

    template<typename OutputIterator>
    void copy_hands(OutputIterator o) const {
      std::copy(hands_.begin(), hands_.end(), o);
    }

    template<typename OutputIterator>
    void copy_talon(OutputIterator o) const {
      std::copy(talon_.begin(), talon_.end(), o);
    }

    void write(std::ostream&, std::string delimiter) const;
  private:
    void sanity_check() const;

    std::array<Cards, 4> hands_;
    std::array<Cards, 2> talon_;
};

template<typename Range>
Deal::Deal(
    Range chunks,
    typename boost::enable_if<
        typename std::is_same<
          typename boost::range_value<Range>::type,
          Cards
        >::type,
        int
      >::type
  ) {
  if (boost::size(chunks) != 6) {
    throw std::logic_error("wrong number chunks");
  }
  typename boost::range_iterator<Range>::type chunk = chunks.begin();
  for (size_t i=0; i<4; ++i) {
    hands_[i] = *chunk;
    ++chunk;
  }
  for (size_t i=0; i<2; ++i) {
    talon_[i] = *chunk;
    ++chunk;
  }

  sanity_check();
}

template<typename Range1, typename Range2>
Deal::Deal(Range1 hands, Range2 talon) {
  if (boost::size(hands) != 4) {
    throw std::logic_error("wrong number hands");
  }
  if (boost::size(talon) != 2) {
    throw std::logic_error("wrong number talon-halves");
  }
  std::copy(boost::begin(hands), boost::end(hands), hands_);
  std::copy(boost::begin(talon), boost::end(talon), talon_);

  sanity_check();
}

KONIG_API std::ostream& operator<<(std::ostream&, const Deal&);

}

#endif // KONIG__DEAL_HPP

