#ifndef KONIG_AI_OFFENCEAI_HPP
#define KONIG_AI_OFFENCEAI_HPP

#include <boost/spirit/home/phoenix/operator/comparison.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/preprocessor.hpp>

#include <konig/ai/playai.hpp>

namespace konig { namespace ai {

BOOST_PARAMETER_NAME(king_rippiness_penalty)
BOOST_PARAMETER_NAME(trumps_for_unannounced_bird)
BOOST_PARAMETER_NAME(trumps_out_to_abort_pagat)
BOOST_PARAMETER_NAME(trumps_out_to_abort_bird)
BOOST_PARAMETER_NAME(min_desired_rippiness)
BOOST_PARAMETER_NAME(valuable_card_points)
BOOST_PARAMETER_NAME(big_trump_threshold)

namespace detail {

// Helper class for constructor with named parameters
struct OffenceAiImpl {
  template<typename ArgumentPack>
  OffenceAiImpl(ArgumentPack const& args) :
    king_rippiness_penalty_(args[_king_rippiness_penalty | 1]),
    trumps_out_to_abort_pagat_(args[_trumps_out_to_abort_pagat | 2]),
    min_desired_rippiness_(args[_min_desired_rippiness | 4]),
    valuable_card_points_(args[_valuable_card_points | 10]),
    big_trump_threshold_(args[_big_trump_threshold | 10])
  {
    // FIXME: It would be nice to have the vectors initialized above, but I
    // can't make that work; it should be possible with brace-initializers, I
    // think...
    std::vector<Count> const def1{5, 5, 6};
    trumps_for_unannounced_bird_ =
      args[_trumps_for_unannounced_bird | def1];
    std::vector<Count> const def2{4, 6, 10};
    trumps_out_to_abort_bird_ =
      args[_trumps_out_to_abort_bird | def2];
    assert(trumps_for_unannounced_bird_.size() == 3);
    assert(std::adjacent_find(
        trumps_for_unannounced_bird_.begin(),
        trumps_for_unannounced_bird_.end(),
        arg1 > arg2
      ) == trumps_for_unannounced_bird_.end());
    assert(std::adjacent_find(
        trumps_out_to_abort_bird_.begin(), trumps_out_to_abort_bird_.end(),
        arg1 > arg2
      ) == trumps_out_to_abort_bird_.end());
  }

  typedef int8_t Rippiness;
  typedef uint8_t Count;

  Rippiness const king_rippiness_penalty_;
  std::vector<Count> trumps_for_unannounced_bird_;
  Count const trumps_out_to_abort_pagat_;
  std::vector<Count> trumps_out_to_abort_bird_;
  Rippiness const min_desired_rippiness_;
  CardPoints const valuable_card_points_;
  TrumpRank const big_trump_threshold_;
};

}

class OffenceAi : public PlayAi, private detail::OffenceAiImpl {
  public:
    using detail::OffenceAiImpl::Rippiness;
    using detail::OffenceAiImpl::Count;

    BOOST_PARAMETER_CONSTRUCTOR(
      OffenceAi,
      (detail::OffenceAiImpl),
      tag,
      (optional
        (king_rippiness_penalty, (Rippiness))
        (trumps_for_unannounced_bird, (std::vector<Count>))
        (trumps_out_to_abort_pagat, (Count))
        (trumps_out_to_abort_bird, (std::vector<Count>))
        (min_desired_rippiness, (Rippiness))
        (valuable_card_points, (CardPoints))
      )
    )

    virtual void reset(FateAi const&);
    virtual Card play_card(FateAi const&);
  private:
    uint8_t num_voids_;
    std::array<Rippiness, 4> rippiness_;
    std::array<bool, 4> void_waiting_;
    Card lowest_trump_to_rough_;
    Card lowest_trump_to_lead_;
};

}}

#endif // KONIG_AI_OFFENCEAI_HPP

