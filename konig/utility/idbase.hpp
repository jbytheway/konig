#ifndef KONIG__UTILITY__IDBASE_HPP
#define KONIG__UTILITY__IDBASE_HPP

#include <boost/mpl/if.hpp>
#include <boost/lexical_cast.hpp>

namespace konig { namespace utility {

/** \brief Helper class to allow id-style classes to be easily constructed.
 *
 * (lifted from sakusen)
 *
 * Classes can inherit from this class and have semantics appropriate to an id.
 * The template parameters should be some integral type and the type of the
 * derived class itself.  See PlayerId for an example.
 *
 * IdBase adds as much appropriate functionality as it can to the derived
 * class through the use of friend function definitions and static methods.
 *
 * Thanks to the way boost::hash works we can also have hashes work
 * automatically for all derived classes.
 *
 * It is guaranteed that if an instance is default-constructed and then
 * incremented, all values obtained will be distinct until an invalid value is
 * reached (as determined by the \c valid method); after that duplicates may be
 * observed.
 */
template<typename TInteger, typename TDerived>
class IdBase {
  BOOST_STATIC_ASSERT((std::numeric_limits<TInteger>::is_integer));
  friend class boost::serialization::access;
  public:
    /** \brief Type which this is a wrapper for. */
    typedef TInteger internal_type;
    /** \brief Implicit cast to internal type.
     *
     * This allows this to be treated as an integer for such things as
     * comparisons and array indexing. */
    operator TInteger() const { return val_; }
    /** \brief Determine if this id is valid. */
    bool valid() const { return val_ != TInteger(-1); }
    /** \brief Convert to string.
     *
     * \see IdBase::from_string */
    std::string to_string() const {
      // Protect against unhelpful lexical_casts of chars
      typedef typename boost::mpl::if_c<
        sizeof(TInteger) == 1,
        int,
        TInteger
      >::type SafeType;
      return boost::lexical_cast<std::string>(SafeType(val_));
    }

    /** \brief Construct an invalid id. */
    static TDerived invalid() {
      TDerived temp;
      temp.val_ = TInteger(-1);
      return temp;
    }
    /** \brief Construct from string.
     *
     * It is guaranteed that converting an id to a string and back again will
     * yield the original value.
     *
     * \see IdBase::to_string. */
    static TDerived from_string(const std::string& s) {
      return from_integer(boost::lexical_cast<TInteger>(s));
    }
    /** \brief Construct from internal integer representation.
     *
     * The need to use this function is a bad sign; it shows excessive
     * dependence on the internal representation. */
    static TDerived from_integer(const TInteger i) {
      TDerived b;
      b.val_ = i;
      return b;
    }

    friend TDerived& operator++(TDerived& c) { ++c.val_; return c; }
    friend TDerived operator++(TDerived& b, int) {
      TDerived temp(b); ++b; return temp;
    }
    friend bool operator==(TDerived l, TDerived r) { return l.val_ == r.val_; }
    friend bool operator!=(TDerived l, TDerived r) { return l.val_ != r.val_; }

    template<typename Archive>
    friend void serialize(Archive& archive, TDerived& t, const unsigned int) {
      archive & boost::serialization::make_nvp("val", t.val_);
    }
  protected:
    /** \brief Default constructor always produces the same, valid id */
    IdBase() : val_(0) {}
    IdBase(IdBase const&) = default;
    IdBase& operator=(IdBase const&) = default;
  private:
    TInteger val_;
};

/* The name hash_value for this function is required for it to work with
 * boost::hash */
template<typename TInteger, typename TDerived>
inline size_t hash_value(const IdBase<TInteger, TDerived>& id)
{
  boost::hash<TInteger> intHasher;
  return intHasher(id);
}

}}

#endif // KONIG__UTILITY__IDBASE_HPP

