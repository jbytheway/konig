#include "client.hpp"

#include <iostream>

#include <konig/fatal.hpp>

#include "server.hpp"

namespace konig { namespace server {

Client::~Client()
{
  connection_->reset_callbacks();
  connection_->close();
}

#define KONIG_SERVER_CLIENT_IGNORE(type)              \
void Client::message(const Message<type>&) {          \
  std::ostringstream os;                              \
  os << "warning: ignoring message of type " << type; \
  server_.warning(os.str());                          \
}
KONIG_SERVER_CLIENT_IGNORE(MessageType::joined)
KONIG_SERVER_CLIENT_IGNORE(MessageType::rejection)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifySetting)
KONIG_SERVER_CLIENT_IGNORE(MessageType::startGame)
KONIG_SERVER_CLIENT_IGNORE(MessageType::requestBid)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifyBid)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifyContractEstablished)
KONIG_SERVER_CLIENT_IGNORE(MessageType::requestCallKing)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifyCallKing)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifyTalon)
KONIG_SERVER_CLIENT_IGNORE(MessageType::requestTalonChoice)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifyTalonChoice)
KONIG_SERVER_CLIENT_IGNORE(MessageType::requestDiscard)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifyDiscard)
KONIG_SERVER_CLIENT_IGNORE(MessageType::requestAnnouncements)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifyAnnouncements)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifyAnnouncementsDone)
KONIG_SERVER_CLIENT_IGNORE(MessageType::requestPlayCard)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifyPlayCard)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifyInvalid)
#undef KONIG_SERVER_CLIENT_IGNORE

void Client::message(const Message<MessageType::getSetting>& m)
{
  std::string reason;
  std::set<std::string> value;
  settingstree::node const* node;
  boost::tie(reason, value, node) =
    server_.get_request(*this, m.get<fields::name>());
  if (reason.empty()) {
    send(Message<MessageType::notifySetting>(node->full_name(), value));
  } else {
    send(Message<MessageType::rejection>(std::move(reason)));
  }
}

void Client::message(const Message<MessageType::setSetting>& m)
{
  std::string reason =
    server_.set_request(*this, m.get<fields::name>(), m.get<fields::value>());
  if (!reason.empty()) {
    send(Message<MessageType::rejection>(std::move(reason)));
  }
}

template<typename T>
static void remote_return(
    Client& client,
    std::type_info const& expected_type,
    boost::any& return_value,
    T&& val
  )
{
  if (expected_type != typeid(T)) {
    client.send(Message<MessageType::rejection>(
          std::string("unexpected message")
        ));
  } else if (!return_value.empty()) {
    KONIG_FATAL("too many returned values");
  } else {
    return_value = std::forward<T>(val);
  }
}

#define KONIG_SERVER_CLIENT_REMOTE_RETURN(type, field)    \
void Client::message(const Message<MessageType::type>& m) \
{                                                         \
  remote_return(                                          \
      *this, *expected_remote_return_type_, remote_return_value_, \
      m.get<fields::field>()                              \
    );                                                    \
}
KONIG_SERVER_CLIENT_REMOTE_RETURN(bid, bid)
KONIG_SERVER_CLIENT_REMOTE_RETURN(callKing, king)
KONIG_SERVER_CLIENT_REMOTE_RETURN(talonChoice, choice)
KONIG_SERVER_CLIENT_REMOTE_RETURN(discard, discard)
KONIG_SERVER_CLIENT_REMOTE_RETURN(announcements, announcements)
KONIG_SERVER_CLIENT_REMOTE_RETURN(playCard, card)
#undef KONIG_SERVER_CLIENT_REMOTE_RETURN

void Client::error(
    const messaging::error_source es,
    const boost::system::error_code& ec
  )
{
  std::cerr << "error: client: " << es << ": " << ec.message() << std::endl;
  if (aborting_) *aborting_ = true;
  server_.remove_client(this);
}

void Client::close()
{
  connection_->close_gracefully();
}

settingstree::leaf_callback<std::uint8_t>& Client::callback_position()
{
  return callbacks_.position();
}

std::string Client::set_table_position(TablePosition const pos)
{
  table_position_ = pos;
  return "";
}

settingstree::leaf_callback<std::uint8_t>& Client::Callbacks::position()
{
  return *this;
}

std::string Client::Callbacks::setting_altering(
    settingstree::int_leaf<std::uint8_t>&,
    std::uint8_t val
  )
{
  // Altering player
  if (val > 4) {
    return "table position must be at most 4";
  }
  return client_.set_table_position(TablePosition(val));
}

void Client::Callbacks::setting_altered(settingstree::leaf& leaf)
{
  client_.server_.notify_setting(leaf);
}

}}

