#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <konig/utility/smallset.hpp>

#include <set>

#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/range/algorithm/equal.hpp>

namespace konig { namespace utility {

BOOST_AUTO_TEST_CASE(concept_check)
{
  enum Test {
    one = 1,
    max
  };
  enum class Test2 {
    one = 1,
    max
  };
#if 0
  // This was a nice idea and it helped get the implementation right, but alas
  // we cannot even satisfy ForwardContainer because our iterators don't
  // dereference to an lvalue
  BOOST_CONCEPT_ASSERT((
      boost::SimpleAssociativeContainer<SmallSet<int, 7>>));
  BOOST_CONCEPT_ASSERT((
      boost::UniqueAssociativeContainer<SmallSet<int, 7>>));
  BOOST_CONCEPT_ASSERT((
      boost::SortedAssociativeContainer<SmallSet<int, 7>>));
  BOOST_CONCEPT_ASSERT((
      boost::SimpleAssociativeContainer<SmallSet<unsigned int, 64>>));
  BOOST_CONCEPT_ASSERT((
      boost::UniqueAssociativeContainer<SmallSet<unsigned int, 64>>));
  BOOST_CONCEPT_ASSERT((
      boost::SortedAssociativeContainer<SmallSet<unsigned int, 64>>));
  BOOST_CONCEPT_ASSERT((
      boost::SimpleAssociativeContainer<SmallSet<Test, max>>));
  BOOST_CONCEPT_ASSERT((
      boost::UniqueAssociativeContainer<SmallSet<Test, max>>));
  BOOST_CONCEPT_ASSERT((
      boost::SortedAssociativeContainer<SmallSet<Test, max>>));
  BOOST_CONCEPT_ASSERT((
      boost::SimpleAssociativeContainer<SmallSet<Test2, Test2::max>>));
  BOOST_CONCEPT_ASSERT((
      boost::UniqueAssociativeContainer<SmallSet<Test2, Test2::max>>));
  BOOST_CONCEPT_ASSERT((
      boost::SortedAssociativeContainer<SmallSet<Test2, Test2::max>>));
#endif
}

BOOST_AUTO_TEST_CASE(behaviour_64_matches_set)
{
  SmallSet<int, 64> s1;
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
  s1.clear();
  s2.clear();
  BOOST_CHECK(boost::range::equal(s1, s2));
}

BOOST_AUTO_TEST_CASE(behaviour_8_matches_set)
{
  SmallSet<int, 8> s1;
  std::set<int> s2;
  BOOST_CHECK(boost::range::equal(s1, s2));
  s1.insert(0);
  s2.insert(0);
  BOOST_CHECK(boost::range::equal(s1, s2));
}

}}

