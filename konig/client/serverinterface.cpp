#include "serverinterface.hpp"

#include <boost/algorithm/string/join.hpp>

#include <messaging/create_connection.hpp>
#include <messaging/callback_helper.hpp>

#include <konig/fatal.hpp>
#include <konig/protocol.hpp>

namespace konig { namespace client {

ServerInterface::ServerInterface(asio::io_service& io, ClientInterface& cl) :
  client_(cl),
  id_(ClientId::invalid())
{
  connection_ = messaging::create_connection<Protocol>(
      io, asio::ip::tcp::endpoint(
        asio::ip::address_v4::from_string("127.0.0.1"), Protocol::port
      ), messaging::callback_helper<ServerInterface>(*this)
    );
}

ServerInterface::~ServerInterface()
{
  connection_->reset_callbacks();
  connection_->close();
}

void ServerInterface::error(
    const messaging::error_source es,
    const boost::system::error_code& ec
  )
{
  std::ostringstream os;
  os << "error: " << es << ": " << ec.message();
  client_.warning(os.str());
  client_.abort();
  if (connection_) connection_->close();
}

#define KONIG_CLIENT_SERVERINTERFACE_IGNORE(type)     \
void ServerInterface::message(const Message<type>&) { \
  std::ostringstream os;                              \
  os << "warning: ignoring message of type " << type; \
  client_.warning(os.str());                          \
}
KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::getSetting)
KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::setSetting)
KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::bid)
#undef KONIG_CLIENT_SERVERINTERFACE_IGNORE

void ServerInterface::message(Message<MessageType::joined> const& m)
{
  id_ = m.get<fields::id>();
  client_.id_known(id_);
  client_.message("Server assigned us id "+id_.to_string());
}

void ServerInterface::message(Message<MessageType::rejection> const& m)
{
  auto const& reason = m.get<fields::reason>();
  client_.message("Server rejected request: "+reason);
}

void ServerInterface::message(Message<MessageType::notifySetting> const& m)
{
  auto const& name = m.get<fields::name>();
  auto const& value_set = m.get<fields::value>();
  if (value_set.empty()) {
    client_.message("Setting '"+name+"' empty");
  } else {
    std::string values = boost::algorithm::join(value_set, ", ");
    client_.message("Setting '"+name+"' has value '"+values+"'");
  }
}

void ServerInterface::message(Message<MessageType::startGame> const& m)
{
  client_.player().start_game(
      m.get<fields::rules>(), m.get<fields::position>(), m.get<fields::cards>()
    );
}

void ServerInterface::message(Message<MessageType::requestBid> const&)
{
  int bid = client_.player().bid();
  client_.message("Sending bid "+boost::lexical_cast<std::string>(bid));
  send(Message<MessageType::bid>(bid));
}

void ServerInterface::close()
{
  connection_->close_gracefully();
}

}}

