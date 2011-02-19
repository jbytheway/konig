#ifndef KONIG_AI__GET_DANGEROUS_SUIT_CANDIDATES_HPP
#define KONIG_AI__GET_DANGEROUS_SUIT_CANDIDATES_HPP

#include <konig/ai/fateai.hpp>

namespace konig { namespace ai {

std::vector<std::pair<int, Card>> get_dangerous_suit_candidates(FateAi const&);

}}

#endif // KONIG_AI__GET_DANGEROUS_SUIT_CANDIDATES_HPP

