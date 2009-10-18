#ifndef KONIG__MESSAGEDATA_HPP
#define KONIG__MESSAGEDATA_HPP

#include <boost/fusion/include/map.hpp>
#include <boost/serialization/set.hpp>

#include <konig/fields.hpp>
#include <konig/messagetype.hpp>
#include <konig/clientid.hpp>
#include <konig/ruleset.hpp>
#include <konig/playposition.hpp>
#include <konig/cards.hpp>

namespace konig {

template<MessageType::internal_enum>
struct MessageData;

template<>
struct MessageData<MessageType::joined> {
  typedef fusion::map<
    fusion::pair<fields::id, ClientId>
  > type;
};

template<>
struct MessageData<MessageType::rejection> {
  typedef fusion::map<
    fusion::pair<fields::reason, std::string>
  > type;
};

template<>
struct MessageData<MessageType::getSetting> {
  typedef fusion::map<
    fusion::pair<fields::name, std::string>
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

template<>
struct MessageData<MessageType::startGame> {
  typedef fusion::map<
    fusion::pair<fields::rules, Ruleset>,
    fusion::pair<fields::position, PlayPosition>,
    fusion::pair<fields::cards, Cards>
  > type;
};

template<>
struct MessageData<MessageType::requestBid> {
  typedef fusion::map<
  > type;
};

template<>
struct MessageData<MessageType::bid> {
  typedef fusion::map<
    fusion::pair<fields::bid, int>
  > type;
};

}

#endif // KONIG__MESSAGEDATA_HPP

