#include "serverinterface.hpp"

#include <boost/algorithm/string/join.hpp>

#include <messaging/create_connection.hpp>
#include <messaging/callback_helper.hpp>

#include <konig/fatal.hpp>
#include <konig/protocol.hpp>
#include <konig/client/asynccallerror.hpp>

namespace konig { namespace client {

ServerInterface::ServerInterface(asio::io_service& io, ClientInterface& cl) :
  io_(io),
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
KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::callKing)
KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::talonChoice)
KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::discard)
KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::announcements)
KONIG_CLIENT_SERVERINTERFACE_IGNORE(MessageType::playCard)
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

void ServerInterface::message(Message<MessageType::notifyBid> const& m)
{
  client_.player().notify_bid(m.get<fields::position>(), m.get<fields::bid>());
}

void ServerInterface::message(Message<MessageType::notifyPlayCard> const& m)
{
  client_.player().notify_play_card(
      m.get<fields::position>(), m.get<fields::card>()
    );
}

namespace {

template<
  MessageType::internal_enum response,
  typename Message<response>::only_value (Player::*member)()
>
class Responder {
  public:
    Responder(ServerInterface& i, Player& p) :
      server_interface_(i),
      player_(p)
    {}
    void operator()() {
      try {
        server_interface_.send(Message<response>((player_.*member)()));
      } catch (AsyncCallError const&) {
        server_interface_.close();
      }
    }
  private:
    ServerInterface& server_interface_;
    Player& player_;
};

}

#define KONIG_CLIENT_SERVERINTERFACE_REQUEST(request, response, member) \
void ServerInterface::message(Message<MessageType::request> const&)     \
{                                                                       \
  io_.post(Responder<MessageType::response, &Player::member>(           \
        *this, client_.player()                                         \
      ));                                                               \
}
KONIG_CLIENT_SERVERINTERFACE_REQUEST(requestBid, bid, bid)
KONIG_CLIENT_SERVERINTERFACE_REQUEST(requestCallKing, callKing, call_king)
KONIG_CLIENT_SERVERINTERFACE_REQUEST(
    requestTalonChoice, talonChoice, choose_talon_half
  )
KONIG_CLIENT_SERVERINTERFACE_REQUEST(requestDiscard, discard, discard)
KONIG_CLIENT_SERVERINTERFACE_REQUEST(
    requestAnnouncements, announcements, announce
  )
KONIG_CLIENT_SERVERINTERFACE_REQUEST(requestPlayCard, playCard, play_card)
#undef KONIG_CLIENT_SERVERINTERFACE_REQUEST

#define KONIG_CLIENT_SERVERINTERFACE_NOTIFY(type, member, field)   \
void ServerInterface::message(Message<MessageType::type> const& m) \
{                                                                  \
  client_.player().member(m.get<fields::field>());                 \
}
KONIG_CLIENT_SERVERINTERFACE_NOTIFY(notifyCallKing, notify_call_king, king)
KONIG_CLIENT_SERVERINTERFACE_NOTIFY(notifyTalon, notify_talon, talon)
KONIG_CLIENT_SERVERINTERFACE_NOTIFY(
    notifyTalonChoice, notify_talon_choice, choice
  )
KONIG_CLIENT_SERVERINTERFACE_NOTIFY(notifyDiscard, notify_discard, discard)
KONIG_CLIENT_SERVERINTERFACE_NOTIFY(
    notifyAnnouncements, notify_announcements, announcements
  )
#undef KONIG_CLIENT_SERVERINTERFACE_NOTIFY

void ServerInterface::close()
{
  connection_->close_gracefully();
}

}}

