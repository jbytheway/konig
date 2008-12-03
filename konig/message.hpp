#ifndef KONIG__MESSAGE_HPP
#define KONIG__MESSAGE_HPP

#include <konig/messagedata.hpp>

namespace konig {

template<MessageType::internal_enum Type>
class Message {
  private:
    friend class boost::serialization::access;

    typename MessageData<Type>::type data_;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/) {
      ar & BOOST_SERIALIZATION_NVP(data_);
    }
};

}

#endif // KONIG__MESSAGE_HPP

