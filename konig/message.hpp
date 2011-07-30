#ifndef KONIG_MESSAGE_HPP
#define KONIG_MESSAGE_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/value_at_key.hpp>

#include <konig/messagedata.hpp>

namespace konig {

namespace detail {

// Metafunctions to work around the fact that fusion isn't variadic yet
template<typename... Args>
struct variadic_mpl_vector;

template<>
struct variadic_mpl_vector<>
{
  typedef boost::mpl::vector<>::type type;
};

template<typename First, typename... More>
struct variadic_mpl_vector<First, More...>
{
  typedef typename boost::mpl::push_front<
      typename variadic_mpl_vector<More...>::type,
      First
    >::type type;
};

template<typename... Args>
struct variadic_fusion_vector {
  typedef typename boost::fusion::result_of::as_vector<
      typename variadic_mpl_vector<Args...>::type
    >::type type;
};

}

template<MessageType::internal_enum Type>
class Message {
  private:
    typedef typename MessageData<Type>::type Data;
    typedef typename boost::mpl::transform<
      Data,
      boost::fusion::result_of::second<boost::mpl::_1>,
      boost::mpl::back_inserter<boost::mpl::vector<>::type>
    >::type Values;
  public:
    typedef typename boost::mpl::if_c<
      boost::mpl::size<Values>::type::value == 1,
      typename boost::mpl::front<Values>::type,
      void
    >::type only_value;

    Message() = default; // For serialization
    // Constructor for building out of relevant data
    template<
      typename... T,
      typename = typename boost::enable_if<
        typename boost::mpl::equal<
          Values,
          typename detail::variadic_mpl_vector<
            typename std::remove_const<
              typename std::remove_reference<T>::type
            >::type...
          >::type
        >::type,
        void
      >::type
    >
    Message(T&&... args) : data_(std::forward<T>(args)...) {
      // Following stuff exposes a gcc bug
      /*typedef typename detail::variadic_mpl_vector<
        typename std::remove_const<
          typename std::remove_reference<T>::type
        >::type...
      >::type Args;
      BOOST_MPL_ASSERT((typename boost::mpl::equal<Values, Args>::type));*/
    }

    template<typename Field>
    typename boost::fusion::result_of::value_at_key<Data, Field>::type const&
    get() const { return boost::fusion::at_key<Field>(data_); }
  private:
    friend class boost::serialization::access;

    Data data_;

    template<typename Archive>
    void serialize_impl(Archive& ar, boost::mpl::false_) {
      ar & BOOST_SERIALIZATION_NVP(data_);
    }

    template<typename Archive>
    void serialize_impl(Archive&, boost::mpl::true_) {
      // Data is empty; cannot be serialized because ADL won't find the
      // function in konig::fields.  Luckily it doesn't need to be serialized
    }

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/) {
      serialize_impl(ar, boost::fusion::empty(data_));
    }
};

}

#endif // KONIG_MESSAGE_HPP

