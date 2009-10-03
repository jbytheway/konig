#ifndef KONIG_SERVER__CLIENT_HPP
#define KONIG_SERVER__CLIENT_HPP

#include <boost/shared_ptr.hpp>

#include <messaging/connection.hpp>
#include <messaging/error_source.hpp>
#include <messaging/callback_helper.hpp>
#include <messaging/send.hpp>

#include <konig/message.hpp>
#include <konig/protocol.hpp>

namespace konig { namespace server {

class Server;

class Client : public boost::enable_shared_from_this<Client> {
  public:
    typedef boost::shared_ptr<Client> Ptr;

    template<typename Connection>
    Client(Connection& c, Server& s) :
      server_(s),
      connection_(c.shared_from_this())
    {
      c.reset_callbacks(messaging::callback_helper<Client>(*this));
    }

    ~Client();

    template<typename Message, typename Connection>
    void message(const Message& m, Connection&) {
      message(m);
    }
    void message(const Message<MessageType::setSetting>&);
    void message(const Message<MessageType::notifySetting>&);
    void error(const messaging::error_source, const boost::system::error_code&);

    template<typename Message>
    void send(Message const& m) {
      messaging::send<konig::Protocol>(m, connection_);
    }

    void close();
  private:
    Server& server_;
    messaging::connection::ptr connection_;
};

}}

#endif // KONIG_SERVER__CLIENT_HPP

