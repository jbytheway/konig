#ifndef KONIG_UTILITY__SEED_MERSENNE_TWISTER_HPP
#define KONIG_UTILITY__SEED_MERSENNE_TWISTER_HPP

namespace konig { namespace utility {

template<typename MersenneTwister>
void seed_mersenne_twister(MersenneTwister& mt)
{
  BOOST_MPL_ASSERT_RELATION(MersenneTwister::word_size, ==, 32);
  BOOST_MPL_ASSERT((std::is_same<std::random_device::result_type, uint32_t>));
  std::random_device device("/dev/urandom");
  // TODO: change to complie-time assertions once constexpr implemented
  assert(device.min() == 0);
  assert(device.max() == std::numeric_limits<uint32_t>::max());
  std::array<uint32_t, MersenneTwister::state_size> seed;
  BOOST_FOREACH(auto& s, seed) {
    s = device();
  }
  std::seed_seq seed_seq(seed.begin(), seed.end());
  mt.seed(seed_seq);
}

}}

#endif // KONIG_UTILITY__SEED_MERSENNE_TWISTER_HPP

