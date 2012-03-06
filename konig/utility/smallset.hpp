#ifndef KONIG_UTILITY_SMALLSET_HPP
#define KONIG_UTILITY_SMALLSET_HPP

#include <boost/integer.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <konig/utility/bitops.hpp>

namespace konig { namespace utility {

// A SimpleAssociativeContainer which contains only small non-negative integers
// and is implemented using bits in an unsigned int (in particular, it never
// needs to malloc anything)
template<typename Value, Value Max>
class SmallSet {
  friend class boost::serialization::access;
  public:
    static_assert(
      std::is_integral<Value>::value || std::is_enum<Value>::value,
      "Value type must be integral or enumeration type"
    );
    typedef typename std::make_unsigned<Value>::type integer_value_type;
    static constexpr integer_value_type max_max =
      std::numeric_limits<uintmax_t>::digits;
    static_assert(
      integer_value_type(Max) <= max_max,
      "Max too large or negative"
    );

    // Standard container typedefs
    typedef Value value_type;
    typedef value_type key_type;
    typedef value_type const& const_reference;
    typedef value_type const* const_pointer;
    typedef size_t size_type;
    typedef std::less<key_type> key_compare;
    typedef key_compare value_compare;

    class iterator :
      public boost::iterator_facade<
        iterator,
        value_type,
        boost::bidirectional_traversal_tag,
        value_type
      > {
      public:
        iterator() = default;

        constexpr iterator(
          SmallSet const& set,
          value_type index = value_type(end_index)
        ) :
          set_(&set), index_(index) {}
      private:
        friend class boost::iterator_core_access;

        value_type dereference() const {
          return index_;
        }

        void increment() {
           *this = set_->upper_bound(index_);
        }

        void decrement() {
          storage_type const smaller_bits =
            set_->storage_ & set_->mask(0, index_);
          index_ = value_type(bitops::highest_bit(smaller_bits));
        }

        bool equal(const iterator& r) const {
          assert(set_ == r.set_);
          return index_ == r.index_;
        }

        SmallSet const* set_;
        value_type index_;
    };

    typedef iterator const_iterator;

    typedef boost::reverse_iterator<iterator> reverse_iterator;
    typedef reverse_iterator const_reverse_iterator;

    typedef typename iterator::difference_type difference_type;

    SmallSet(key_compare = key_compare()) : storage_{0} {}

    template<typename Iterator>
    SmallSet(Iterator i1, Iterator i2, key_compare = key_compare()) :
      storage_{0}
    {
      insert(std::move(i1), std::move(i2));
    }

    SmallSet(std::initializer_list<value_type> l) :
      storage_{0}
    {
      insert(l.begin(), l.end());
    }

    key_compare key_comp() const { return {}; }
    value_compare value_comp() const { return {}; }

    bool empty() const { return storage_ == 0; }
    size_type size() const { return bitops::popcount(storage_); }
    static size_type max_size() { return size_type(Max); }

    iterator begin() const { return lower_bound(value_type(0)); }
    iterator end() const { return iterator(*this); }

    reverse_iterator rbegin() const { return reverse_iterator{end()}; }
    reverse_iterator rend() const { return reverse_iterator{begin()}; }

    size_type count(key_type const k) const {
      return (storage_ & mask(k)) != 0;
    }

    iterator lower_bound(key_type const k) const {
      storage_type const not_smaller_bits =
        storage_ & ~mask(0, k);
      auto const index = bitops::trailing_zeros(not_smaller_bits);
      return iterator(*this, value_type(index));
    }

    iterator upper_bound(key_type const k) const {
      storage_type const larger_bits =
        storage_ & ~mask(0, integer_value_type(k)+1);
      auto const index = bitops::trailing_zeros(larger_bits);
      return iterator(*this, value_type(index));
    }

    iterator find(key_type const k) const {
      if (count(k)) {
        return iterator(*this, k);
      } else {
        return end();
      }
    }

    std::pair<iterator, iterator> equal_range(key_type const k) const {
      if (count(k)) {
        return {iterator(*this, k), upper_bound(k)};
      } else {
        return {end(), end()};
      }
    }

    void clear() {
      storage_ = 0;
    }

    std::pair<iterator, bool> insert(value_type const v) {
      bool inserted = !count(v);
      storage_ |= mask(v);
      return {{*this, v}, inserted};
    }

    iterator insert(iterator const&, value_type const v) {
      storage_ |= mask(v);
      return {*this, v};
    }

    template<typename Iterator>
    void insert(Iterator i1, Iterator const i2) {
      for ( ; i1 != i2; ++i1) {
        storage_ |= mask(*i1);
      }
    }

    size_type erase(key_type const k) {
      auto const result = count(k);
      storage_ &= ~mask(k);
      return result;
    }

    iterator erase(iterator const& i) {
      erase(*i);
      // Note that the following would be undefined behaviour if I didn't know
      // implementation details of the iterator
      return upper_bound(*i);
    }

    iterator erase(iterator const& i1, iterator const& i2) {
      storage_ &= ~mask(*i1, *i2);
      return i2;
    }

    // Returns subset between given keys
    SmallSet subset(key_type const& k1, key_type const& k2) const
    {
      return SmallSet{storage_ & mask(k1, k2), from_storage()};
    }

    // Returns subset between given iterators
    SmallSet subset(iterator const& i1, iterator const& i2) const
    {
      return SmallSet{storage_ & mask(*i1, *i2), from_storage()};
    }

    // Returns subset after given key
    SmallSet subset_after(key_type const& k) const
    {
      return SmallSet{storage_ & mask(k, end_index), from_storage()};
    }

    friend inline bool intersects(SmallSet const& l, SmallSet const& r) {
      return (l.storage_ & r.storage_) != 0;
    }

    friend inline void intersection_into(SmallSet& l, SmallSet const& r) {
      l.storage_ &= r.storage_;
    }

    friend inline void union_into(SmallSet& l, SmallSet const& r) {
      l.storage_ |= r.storage_;
    }

    friend inline bool operator==(SmallSet const& l, SmallSet const& r) {
      return l.storage_ == r.storage_;
    }
  private:
    // Typedefs specific to SmallSet
    typedef typename boost::uint_t<int(Max)>::least storage_type;

    static constexpr integer_value_type end_index =
      std::numeric_limits<storage_type>::digits;

    // Internal constructor to make directly from a storage value
    struct from_storage {};
    SmallSet(storage_type const s, from_storage) : storage_{s} {}

    template<typename T>
    static storage_type mask(T const k) {
      auto const i_k = integer_value_type(k);
      assert(i_k < max_size());
      return storage_type(1) << i_k;
    }

    template<typename T, typename U>
    static storage_type mask(T const k1, U const k2) {
      auto const i_k1 = integer_value_type(k1);
      auto const i_k2 = integer_value_type(k2);
      assert(i_k2 <= max_size() || i_k2 == end_index);
      assert(i_k1 <= i_k2);
      integer_value_type const span = i_k2 - i_k1;
      return bitops::ones<storage_type>(span) << i_k1;
    }

    template<typename Archive>
    void serialize(Archive& ar, unsigned int) {
      ar & BOOST_SERIALIZATION_NVP(storage_);
    }

    storage_type storage_;
};

}}

#endif // KONIG_UTILITY_SMALLSET_HPP

