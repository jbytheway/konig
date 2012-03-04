#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <konig/utility/transformingset.hpp>

#include <set>

#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/range/algorithm/equal.hpp>

namespace konig { namespace utility {

namespace {

  struct Upcast {
    int operator()(char i) const { return i; }
  };

  struct Downcast {
    char operator()(int i) const { return i; }
  };

}

BOOST_AUTO_TEST_CASE(transformingset_concept_check)
{
  typedef TransformingSet<std::set<char>, Upcast, Downcast> Set;
  BOOST_CONCEPT_ASSERT((
      boost::SimpleAssociativeContainer<Set>));
  BOOST_CONCEPT_ASSERT((
      boost::UniqueAssociativeContainer<Set>));
  BOOST_CONCEPT_ASSERT((
      boost::SortedAssociativeContainer<Set>));
}

BOOST_AUTO_TEST_CASE(transformingset_behaviour_matches_set)
{
  TransformingSet<std::set<char>, Upcast, Downcast> s1;
  std::set<int> s2;
  BOOST_CHECK(boost::range::equal(s1, s2));
  s1.insert(0);
  s2.insert(0);
  BOOST_CHECK(boost::range::equal(s1, s2));
  s1.insert(3);
  s2.insert(3);
  BOOST_CHECK(boost::range::equal(s1, s2));
  BOOST_CHECK_EQUAL(s1.insert(3).second, s2.insert(3).second);
  BOOST_CHECK(boost::range::equal(s1, s2));
  BOOST_CHECK_EQUAL(*s1.insert(63).first, *s2.insert(63).first);
  BOOST_CHECK(boost::range::equal(s1, s2));
  BOOST_CHECK_EQUAL(s1.erase(7), s2.erase(7));
  BOOST_CHECK(boost::range::equal(s1, s2));
  BOOST_CHECK_EQUAL(s1.erase(0), s2.erase(0));
  BOOST_CHECK(boost::range::equal(s1, s2));
  BOOST_CHECK_EQUAL(*s1.erase(s1.begin()), *s2.erase(s2.begin()));
  BOOST_CHECK(boost::range::equal(s1, s2));
}

}}

