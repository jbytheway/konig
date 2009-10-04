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
KONIG_SERVER_CLIENT_IGNORE(MessageType::rejection)
KONIG_SERVER_CLIENT_IGNORE(MessageType::notifySetting)
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

void Client::error(
    const messaging::error_source es,
    const boost::system::error_code& ec
  )
{
  std::cerr << "error: client: " << es << ": " << ec.message() << std::endl;
  server_.remove_client(this);
}

void Client::close()
{
  connection_->close_gracefully();
}

}}

