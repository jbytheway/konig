#ifndef KONIG_SERVER__CLIENT_HPP
#define KONIG_SERVER__CLIENT_HPP

#include <boost/shared_ptr.hpp>

#include <messaging/connection.hpp>
#include <messaging/error_source.hpp>
#include <messaging/callback_helper.hpp>
#include <messaging/send.hpp>

#include <settingstree/user.hpp>
#include <settingstree/leaf_callback.hpp>

#include <konig/message.hpp>
#include <konig/protocol.hpp>
#include <konig/clientid.hpp>
#include <konig/tableposition.hpp>

namespace konig { namespace server {

class Server;

class Client : private settingstree::user {
  public:
    template<typename Connection>
    Client(Connection& c, Server& s, ClientId id) :
      settingstree::user("client"+id.to_string()),
      id_(id),
      table_position_(0),
      server_(s),
      connection_(c.shared_from_this()),
      callbacks_(*this)
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

    settingstree::leaf_callback<std::uint8_t>& callback_position();
    std::string set_table_position(TablePosition);
  private:
    ClientId id_;
    TablePosition table_position_;
    Server& server_;
    messaging::connection::ptr connection_;

    class Callbacks : settingstree::leaf_callback<std::uint8_t> {
      public:
        Callbacks(Client& c) : client_(c) {}
        settingstree::leaf_callback<std::uint8_t>& position();
      private:
        virtual std::string setting_altering(
            settingstree::int_leaf<std::uint8_t>&,
            std::uint8_t
          );
        virtual void setting_altered(settingstree::leaf&);
        Client& client_;
    };
    Callbacks callbacks_;
};

}}

#endif // KONIG_SERVER__CLIENT_HPP

