#ifndef KONIG_UTILITY_TRANSFORMINGSET_HPP
#define KONIG_UTILITY_TRANSFORMINGSET_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/utility/intersects.hpp>
#include <konig/utility/intersection_into.hpp>
#include <konig/utility/union_into.hpp>

namespace konig { namespace utility {

namespace detail {

template<typename F, typename UnderlyingCompare>
class ForwardingCompare {
  public:
    ForwardingCompare() :
      underlying_{}
    {}

    ForwardingCompare(UnderlyingCompare u) :
      underlying_{std::move(u)}
    {}

    template<typename T>
    bool operator()(T const& t1, T const& t2) const {
      F const f;
      return underlying_(f(t1), f(t2));
    }

    UnderlyingCompare const& underlying() const { return underlying_; }
  private:
    UnderlyingCompare underlying_;
};

}

template<
  typename UnderlyingSet,
  typename InternalToExternal,
  typename ExternalToInternal
>
class TransformingSet {
  friend class boost::serialization::access;
  public:
    typedef typename UnderlyingSet::value_type underlying_value_type;
    typedef typename UnderlyingSet::iterator underlying_iterator;
    typedef typename std::result_of<
      InternalToExternal(underlying_value_type)
    >::type value_type;
    typedef typename UnderlyingSet::size_type size_type;
    typedef value_type const& const_reference;
    typedef value_type const* const_pointer;
    typedef value_type key_type;
    typedef detail::ForwardingCompare<
      ExternalToInternal,
      typename UnderlyingSet::key_compare
    > key_compare;
    typedef key_compare value_compare;

    class iterator :
      public boost::iterator_facade<
        iterator,
        value_type,
        boost::bidirectional_traversal_tag,
        value_type,
        typename underlying_iterator::difference_type
      > {
      public:
        iterator() = default;

        iterator(underlying_iterator i) :
          underlying_{std::move(i)}
        {}

        underlying_iterator const& underlying() const { return underlying_; }
      private:
        friend class boost::iterator_core_access;

        value_type dereference() const {
          return out_cast(*underlying_);
        }

        bool equal(iterator const& other) const {
          return underlying_ == other.underlying_;
        }

        void increment() {
          ++underlying_;
        }

        void decrement() {
          --underlying_;
        }

        underlying_iterator underlying_;
    };

    typedef iterator const_iterator;

    typedef boost::reverse_iterator<iterator> reverse_iterator;
    typedef reverse_iterator const_reverse_iterator;

    typedef typename iterator::difference_type difference_type;

    TransformingSet(key_compare k = key_compare()) :
      underlying_{k.underlying()}
    {}

    TransformingSet(UnderlyingSet u) : underlying_{std::move(u)} {}

    template<typename Iterator>
    TransformingSet(Iterator i1, Iterator i2, key_compare k = key_compare()) :
      underlying_{k.underlying()}
    {
      insert(std::move(i1), std::move(i2));
    }

    TransformingSet(std::initializer_list<value_type> l) :
      underlying_{}
    {
      insert(l.begin(), l.end());
    }

    key_compare key_comp() const { return {underlying_.key_comp()}; }
    value_compare value_comp() const { return {underlying_.value_comp()}; }

    bool empty() const { return underlying_.empty(); }
    size_type max_size() const { return underlying_.max_size(); }
    size_type size() const { return underlying_.size(); }

    iterator begin() const { return {underlying_.begin()}; }
    iterator end() const { return {underlying_.end()}; }

    reverse_iterator rbegin() const { return reverse_iterator{end()}; }
    reverse_iterator rend() const { return reverse_iterator{begin()}; }

    size_type count(key_type const& k) const {
      return underlying_.count(in_cast(k));
    }

    iterator find(key_type const& k) const {
      return {underlying_.find(in_cast(k))};
    }

    iterator upper_bound(key_type const& k) const {
      return {underlying_.upper_bound(in_cast(k))};
    }

    iterator lower_bound(key_type const& k) const {
      return {underlying_.lower_bound(in_cast(k))};
    }

    std::pair<iterator, iterator> equal_range(key_type const& k) const {
      auto r = underlying_.equal_range(in_cast(k));
      return {{r.first}, {r.second}};
    }

    void clear() {
      underlying_.clear();
    }

    std::pair<iterator, bool> insert(value_type const& v) {
      auto r = underlying_.insert(in_cast(v));
      return {{r.first}, r.second};
    }

    iterator insert(iterator const& i, value_type const v) {
      return {underlying_.insert(i.underlying(), in_cast(v))};
    }

    template<typename Iterator>
    void insert(Iterator i1, Iterator const i2) {
      for ( ; i1 != i2; ++i1) {
        insert(*i1);
      }
    }

    size_type erase(key_type const k) {
      return underlying_.erase(in_cast(k));
    }

    iterator erase(iterator const& i) {
      return {underlying_.erase(i.underlying())};
    }

    iterator erase(iterator const& i1, iterator const& i2) {
      return {underlying_.erase(i1.underlying(), i2.underlying())};
    }

    // Returns subset between given keys
    TransformingSet subset(key_type const& k1, key_type const& k2) const
    {
      return {underlying_.subset(in_cast(k1), in_cast(k2))};
    }

    // Returns subset between given iterators
    TransformingSet subset(iterator const& i1, iterator const& i2) const
    {
      return {underlying_.subset(i1.underlying(), i2.underlying())};
    }

    // Returns subset after given key
    TransformingSet subset_after(key_type const& k) const
    {
      return {underlying_.subset_after(in_cast(k))};
    }

    friend inline bool intersects(
      TransformingSet const& l,
      TransformingSet const& r
    ) {
      using utility::intersects;
      return intersects(l.underlying_, r.underlying_);
    }

    friend inline void intersection_into(
      TransformingSet& l,
      TransformingSet const& r
    ) {
      using utility::intersection_into;
      return intersection_into(l.underlying_, r.underlying_);
    }

    friend inline void union_into(
      TransformingSet& l,
      TransformingSet const& r
    ) {
      using utility::union_into;
      return union_into(l.underlying_, r.underlying_);
    }

    friend inline bool operator==(
      TransformingSet const& l,
      TransformingSet const& r
    ) {
      return l.underlying_ == r.underlying_;
    }
  private:
    static underlying_value_type in_cast(value_type const& v) {
      ExternalToInternal f;
      return f(v);
    }

    static value_type out_cast(underlying_value_type const& v) {
      InternalToExternal f;
      return f(v);
    }

    template<typename Archive>
    void serialize(Archive& ar, unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(underlying_);
    }

    UnderlyingSet underlying_;
};

}}

#endif // KONIG_UTILITY_TRANSFORMINGSET_HPP

