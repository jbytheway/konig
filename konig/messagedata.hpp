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
    fusion::pair<fields::bid, Bid>
  > type;
};

template<>
struct MessageData<MessageType::notifyBid> {
  typedef fusion::map<
    fusion::pair<fields::position, PlayPosition>,
    fusion::pair<fields::bid, Bid>
  > type;
};

template<>
struct MessageData<MessageType::notifyContractEstablished> {
  typedef fusion::map<
    fusion::pair<fields::bid, Bid>
  > type;
};

template<>
struct MessageData<MessageType::requestCallKing> {
  typedef fusion::map<
  > type;
};

template<>
struct MessageData<MessageType::callKing> {
  typedef fusion::map<
    fusion::pair<fields::king, KingCall>
  > type;
};

template<>
struct MessageData<MessageType::notifyCallKing> {
  typedef fusion::map<
    fusion::pair<fields::king, KingCall>
  > type;
};

template<>
struct MessageData<MessageType::notifyTalon> {
  typedef fusion::map<
    fusion::pair<fields::talon, std::array<Cards, 2>>
  > type;
};

template<>
struct MessageData<MessageType::requestConcessionChoice> {
  typedef fusion::map<
  > type;
};

template<>
struct MessageData<MessageType::concessionChoice> {
  typedef fusion::map<
    fusion::pair<fields::choice, bool>
  > type;
};

template<>
struct MessageData<MessageType::notifyConcession> {
  typedef fusion::map<
  > type;
};

template<>
struct MessageData<MessageType::requestTalonChoice> {
  typedef fusion::map<
  > type;
};

template<>
struct MessageData<MessageType::talonChoice> {
  typedef fusion::map<
    fusion::pair<fields::choice, uint8_t>
  > type;
};

template<>
struct MessageData<MessageType::notifyTalonChoice> {
  typedef fusion::map<
    fusion::pair<fields::choice, uint8_t>
  > type;
};

template<>
struct MessageData<MessageType::requestDiscard> {
  typedef fusion::map<
  > type;
};

template<>
struct MessageData<MessageType::discard> {
  typedef fusion::map<
    fusion::pair<fields::discard, Cards>
  > type;
};

template<>
struct MessageData<MessageType::notifyDiscard> {
  typedef fusion::map<
    fusion::pair<fields::discard, Cards>
  > type;
};

template<>
struct MessageData<MessageType::requestAnnouncements> {
  typedef fusion::map<
  > type;
};

template<>
struct MessageData<MessageType::announcements> {
  typedef fusion::map<
    fusion::pair<fields::announcements, std::vector<Announcement>>
  > type;
};

template<>
struct MessageData<MessageType::notifyAnnouncements> {
  typedef fusion::map<
    fusion::pair<fields::announcements, std::vector<Announcement>>
  > type;
};

template<>
struct MessageData<MessageType::notifyAnnouncementsDone> {
  typedef fusion::map<
  > type;
};

template<>
struct MessageData<MessageType::requestPlayCard> {
  typedef fusion::map<
  > type;
};

template<>
struct MessageData<MessageType::playCard> {
  typedef fusion::map<
    fusion::pair<fields::card, Card>
  > type;
};

template<>
struct MessageData<MessageType::notifyPlayCard> {
  typedef fusion::map<
    fusion::pair<fields::position, PlayPosition>,
    fusion::pair<fields::card, Card>
  > type;
};

template<>
struct MessageData<MessageType::notifyOuvert> {
  typedef fusion::map<
    fusion::pair<fields::hand, Cards>
  > type;
};

template<>
struct MessageData<MessageType::notifyInvalid> {
  typedef fusion::map<
    fusion::pair<fields::message, std::string>
  > type;
};

}

#endif // KONIG__MESSAGEDATA_HPP

