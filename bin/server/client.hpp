#ifndef KONIG_SERVER__CLIENT_HPP
#define KONIG_SERVER__CLIENT_HPP

#include <boost/shared_ptr.hpp>

#include <messaging/connection.hpp>
#include <messaging/error_source.hpp>
#include <messaging/callback_helper.hpp>
#include <messaging/send.hpp>

#include <settingstree/user.hpp>

#include <konig/message.hpp>
#include <konig/protocol.hpp>
#include <konig/clientid.hpp>

namespace konig { namespace server {

class Server;

class Client : private settingstree::user {
  public:
    template<typename Connection>
    Client(Connection& c, Server& s, ClientId id) :
      settingstree::user("client"+id.to_string()),
      id_(id),
      server_(s),
      connection_(c.shared_from_this())
    {
      c.reset_callbacks(messaging::callback_helper<Client>(*this));
    }

    ~Client();

    ClientId id() const { return id_; }

    template<typename Message, typename Connection>
    void message(const Message& m, Connection&) {
      message(m);
    }
    void message(const Message<MessageType::rejection>&);
    void message(const Message<MessageType::getSetting>&);
    void message(const Message<MessageType::setSetting>&);
    void message(const Message<MessageType::notifySetting>&);
    void error(const messaging::error_source, const boost::system::error_code&);

    template<typename Message>
    void send(Message const& m) {
      messaging::send<konig::Protocol>(m, connection_);
    }

    void close();
  private:
    ClientId id_;
    Server& server_;
    messaging::connection::ptr connection_;
};

}}

#endif // KONIG_SERVER__CLIENT_HPP

