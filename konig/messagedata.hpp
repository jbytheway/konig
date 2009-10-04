#ifndef KONIG__MESSAGEDATA_HPP
#define KONIG__MESSAGEDATA_HPP

#include <boost/fusion/include/map.hpp>
#include <boost/serialization/set.hpp>

#include <konig/fields.hpp>
#include <konig/messagetype.hpp>

namespace konig {

template<MessageType::internal_enum>
struct MessageData;

template<>
struct MessageData<MessageType::rejection> {
  typedef fusion::map<
    fusion::pair<fields::reason, std::string>
  > type;
};

template<>
struct MessageData<MessageType::setSetting> {
  typedef fusion::map<
    fusion::pair<fields::name, std::string>,
    fusion::pair<fields::value, std::string>
  > type;
};

template<>
struct MessageData<MessageType::notifySetting> {
  typedef fusion::map<
    fusion::pair<fields::name, std::string>,
    fusion::pair<fields::value, std::set<std::string>>
  > type;
};

}

#endif // KONIG__MESSAGEDATA_HPP

