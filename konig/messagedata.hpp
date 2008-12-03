#ifndef KONIG__MESSAGEDATA_HPP
#define KONIG__MESSAGEDATA_HPP

#include <boost/fusion/include/map.hpp>

#include <konig/fields.hpp>
#include <konig/messagetype.hpp>

namespace konig {

template<MessageType::internal_enum>
struct MessageData;

template<>
struct MessageData<MessageType::setPlayerProperties> {
  typedef fusion::map<
    fusion::pair<fields::name, std::string>
  > type;
};

}

#endif // KONIG__MESSAGEDATA_HPP

